#include "speed_test_utils.h"

int get_ipv4_addr(char *domain_name, struct sockaddr_in *servinfo)
{
    struct addrinfo hints, *addrinfo, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char *token = strtok(domain_name, ":");

    if ((status = getaddrinfo(token, "http", &hints, &addrinfo)) != 0)
    {
        printf("Resolve DNS Failed: Can't get ip address! (%s)\n", token);
        return 0;
    }

    for (p = addrinfo; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET)
        {
            memcpy(servinfo, (struct sockaddr_in *)p->ai_addr, sizeof(struct sockaddr_in));
        }
    }
    freeaddrinfo(addrinfo);
    return 1;
}

// Setting the port to 443 for HTTPS connections.
int get_ipv4_addr_https(char *domain_name, struct sockaddr_in *servinfo)
{
    struct addrinfo hints, *addrinfo, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char *token = strtok(domain_name, ":");

    if ((status = getaddrinfo(token, "https", &hints, &addrinfo)) != 0)
    {
        printf("Resolve DNS Failed: Can't get ip address! (%s)\n", token);
        return 0;
    }

    for (p = addrinfo; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET)
        {
            memcpy(servinfo, (struct sockaddr_in *)p->ai_addr, sizeof(struct sockaddr_in));
            servinfo->sin_port = htons(443); // Set the port to 443 for HTTPS
            break;
        }
    }

    if (p == NULL)
    {
        printf("Can't get IPv4 address for %s\n", domain_name);
        freeaddrinfo(addrinfo);
        return 0;
    }

    freeaddrinfo(addrinfo);
    return 1;
}

int get_http_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename)
{
    int fd;
    char sbuf[256] = {0}, tmp_path[128] = {0};
    char rbuf[8192];
    struct timeval tv;
    fd_set fdSet;
    FILE *fp = NULL;
    
    if ((fd = socket(serv->sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        if (fd)
            close(fd);
        return 0;
    }
    
    if (connect(fd, (struct sockaddr *)serv, sizeof(struct sockaddr)) == -1)
    {
        // If freeze when using http, the error is here at the connect system call
        perror("Socket connect error!\n");
        if (fd)
            close(fd);
        return 0;
    }
    
    sprintf(sbuf,
            "GET /%s HTTP/1.0\r\n"
            "Host: %s\r\n"
            "User-Agent: status\r\n"
            "Accept: */*\r\n\r\n",
            request_url, domain_name);

    if (send(fd, sbuf, str_len(sbuf), 0) != str_len(sbuf))
    {
        perror("Can't send data to server\n");
        if (fd)
            close(fd);
        return 0;
    }

    sprintf(tmp_path, "%s%s", FILE_DIRECTORY_PATH, filename);
    fp = fopen(tmp_path, "w+");
    
    while (1)
    {
        char *ptr = NULL;
        memset(rbuf, 0, sizeof(rbuf));
        FD_ZERO(&fdSet);
        FD_SET(fd, &fdSet);

        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int status = select(fd + 1, &fdSet, NULL, NULL, &tv);
        int i = recv(fd, rbuf, sizeof(rbuf), 0);
        if (status > 0 && FD_ISSET(fd, &fdSet))
        {
            if (i < 0)
            {
                printf("Can't get http file!\n");
                close(fd);
                fclose(fp);
                return 0;
            }
            else if (i == 0)
            {
                break;
            }
            else
            {
                if ((ptr = strstr(rbuf, "\r\n\r\n")) != NULL)
                {
                    ptr += 4;
                    fwrite(ptr, 1, str_len(ptr), fp);
                }
                else
                {
                    fwrite(rbuf, 1, i, fp);
                }
            }
        }
    }
    close(fd);
    fclose(fp);
    return 1;
}

int get_https_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename)
{
    int fd;
    char sbuf[256] = {0}, tmp_path[128] = {0};
    char rbuf[8192];
    struct timeval tv;
    fd_set fdSet;
    FILE *fp = NULL;

    SSL_CTX *ctx;
    SSL *ssl;

    SSL_library_init();
    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx)
    {
        perror("SSL_CTX_new error!\n");
        return 0;
    }

    if ((fd = socket(serv->sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    if (connect(fd, (struct sockaddr *)serv, sizeof(struct sockaddr)) == -1)
    {
        perror("Socket connect error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, fd);
    if (SSL_connect(ssl) == -1)
    {
        perror("SSL_connect error!\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(fd);
        return 0;
    }

    sprintf(sbuf,
            "GET /%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "User-Agent: status\r\n"
            "Accept: */*\r\n"
            "Connection: close\r\n\r\n",
            request_url, domain_name);

    if (SSL_write(ssl, sbuf, str_len(sbuf)) != str_len(sbuf))
    {
        perror("Can't send data to server\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(fd);
        return 0;
    }

    sprintf(tmp_path, "%s%s", FILE_DIRECTORY_PATH, filename);
    fp = fopen(tmp_path, "w+");

    while (1)
    {
        char *ptr = NULL;
        memset(rbuf, 0, sizeof(rbuf));
        FD_ZERO(&fdSet);
        FD_SET(fd, &fdSet);

        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int status = select(fd + 1, &fdSet, NULL, NULL, &tv);
        int i = SSL_read(ssl, rbuf, sizeof(rbuf));
        if (status > 0 && FD_ISSET(fd, &fdSet))
        {
            if (i < 0)
            {
                printf("Can't get https file!\n");
                SSL_free(ssl);
                SSL_CTX_free(ctx);
                close(fd);
                fclose(fp);
                return 0;
            }
            else if (i == 0)
            {
                break;
            }
            else
            {
                if ((ptr = strstr(rbuf, "\r\n\r\n")) != NULL)
                {
                    ptr += 4;
                    fwrite(ptr, 1, str_len(ptr), fp);
                }
                else
                {
                    fwrite(rbuf, 1, i, fp);
                }
            }
        }
    }

    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(fd);
    fclose(fp);
    return 1;
}

// Haversine formula
double calcDistance(double lat1, double lon1, double lat2, double lon2)
{
    int R = 6371; // Radius of the Earth
    double dlat, dlon, a, c, d;

    dlat = (lat2 - lat1) * M_PI / 180;
    dlon = (lon2 - lon1) * M_PI / 180;

    a = pow(sin(dlat / 2), 2) + cos(lat1 * M_PI / 180) * cos(lat2 * M_PI / 180) * pow(sin(dlon / 2), 2);
    c = 2 * atan2(sqrt(a), sqrt(1 - a));
    d = R * c;
    return d;
}