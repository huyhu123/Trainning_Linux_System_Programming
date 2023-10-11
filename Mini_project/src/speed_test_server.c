#include "speed_test_server.h"
#include "speed_test_utils.h"

int get_ip_address_position(char *fileName, client_data_t *client_data)
{
    FILE *fp = NULL;
    char filePath[128] = {0}, line[512] = {0}, lat[64] = {0}, lon[64] = {0};
    sprintf(filePath, "%s%s", FILE_DIRECTORY_PATH, fileName);

    if ((fp = fopen(filePath, "r")) != NULL)
    {
        while (fgets(line, sizeof(line) - 1, fp) != NULL)
        {
            if (!strncmp(line, "<client", 7))
            {
                // ex: <client ip="61.216.30.97" lat="24.7737" lon="120.9436" isp="HiNet" isprating="3.2" rating="0" ispdlavg="50329" ispulavg="22483" loggedin="0"/>
                sscanf(line, "%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"", client_data->ipAddr, lat, lon, client_data->isp);
                client_data->latitude = atof(lat);
                client_data->longitude = atof(lon);
                break;
            }
        }
        fclose(fp);
    }
    return 1;
}

int get_nearest_server(double lat_c, double lon_c, server_data_t *nearest_servers)
{
    FILE *fp = NULL;
    char filePath[128] = {0}, line[512] = {0}, url[128] = {0}, lat[64] = {0}, lon[64] = {0}, name[128] = {0}, country[128] = {0};
    double lat_s, lon_s, distance;
    int count = 0, i = 0, j = 0;

    clock_t start = 0;
    clock_t end = 0;
    double cpu_time_used = 0;

    sprintf(filePath, "%s%s", FILE_DIRECTORY_PATH, SERVERS_LOCATION_REQUEST_URL);

    if ((fp = fopen(filePath, "r")) != NULL)
    {
        while (fgets(line, sizeof(line) - 1, fp) != NULL)
        {
            if (!strncmp(line, "<server", 7))
            {
                // ex: <server url="http://88.84.191.230/speedtest/upload.php" lat="70.0733" lon="29.7497" name="Vadso" country="Norway" cc="NO" sponsor="Varanger KraftUtvikling AS" id="4600" host="88.84.191.230:8080"/>
                sscanf(line, "%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"", url, lat, lon, name, country);

                lat_s = atof(lat);
                lon_s = atof(lon);

                distance = calcDistance(lat_c, lon_c, lat_s, lon_s);

                for (i = 0; i < NEAREST_SERVERS_NUM; i++)
                {
                    if (nearest_servers[i].url[0] == '\0')
                    {
                        strncpy(nearest_servers[i].url, url, sizeof(url));
                        nearest_servers[i].latitude = lat_s;
                        nearest_servers[i].longitude = lon_s;
                        strncpy(nearest_servers[i].name, name, sizeof(name));
                        strncpy(nearest_servers[i].country, country, sizeof(country));
                        nearest_servers[i].distance = distance;
                        break;
                    }
                    else
                    {
                        if (distance <= nearest_servers[i].distance)
                        {
                            memset(&nearest_servers[NEAREST_SERVERS_NUM - 1], 0, sizeof(server_data_t));
                            for (j = NEAREST_SERVERS_NUM - 1; j > i; j--)
                            {
                                strncpy(nearest_servers[j].url, nearest_servers[j - 1].url, sizeof(nearest_servers[j - 1].url));
                                nearest_servers[j].latitude = nearest_servers[j - 1].latitude;
                                nearest_servers[j].longitude = nearest_servers[j - 1].longitude;
                                strncpy(nearest_servers[j].name, nearest_servers[j - 1].name, sizeof(nearest_servers[j - 1].name));
                                strncpy(nearest_servers[j].country, nearest_servers[j - 1].country, sizeof(nearest_servers[j - 1].country));
                                nearest_servers[j].distance = nearest_servers[j - 1].distance;
                            }
                            strncpy(nearest_servers[i].url, url, sizeof(url));
                            nearest_servers[i].latitude = lat_s;
                            nearest_servers[i].longitude = lon_s;
                            strncpy(nearest_servers[i].name, name, sizeof(name));
                            strncpy(nearest_servers[i].country, country, sizeof(country));
                            nearest_servers[i].distance = distance;
                            break;
                        }
                    }
                }
                count++;
            }
        }
    }
    if (count > 0)
        return 1;
    else
        return 0;
}

int get_nearest_server_https(double lat_c, double lon_c, server_data_t *nearest_servers)
{
    FILE *fp = NULL;
    char filePath[128] = {0}, line[512] = {0}, url[128] = {0}, lat[64] = {0}, lon[64] = {0}, name[128] = {0}, country[128] = {0};
    double lat_s, lon_s, distance;
    int count = 0, i = 0, j = 0;

    clock_t start = 0;
    clock_t end = 0;
    double cpu_time_used = 0;

    sprintf(filePath, "%s%s", FILE_DIRECTORY_PATH, SERVERS_LOCATION_REQUEST_URL);

    // Replace "http" with "https" in the URL
    char https_url[128] = {0};
    strncpy(https_url, filePath, sizeof(filePath));
    char *http_pos = strstr(https_url, "http");
    if (http_pos != NULL) {
        http_pos[4] = 's';
    }

    if ((fp = fopen(https_url, "r")) != NULL)
    {
        while (fgets(line, sizeof(line) - 1, fp) != NULL)
        {
            if (!strncmp(line, "<server", 7))
            {
                // ex: <server url="http://88.84.191.230/speedtest/upload.php" lat="70.0733" lon="29.7497" name="Vadso" country="Norway" cc="NO" sponsor="Varanger KraftUtvikling AS" id="4600" host="88.84.191.230:8080"/>
                sscanf(line, "%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"%*[^\"]\"%255[^\"]\"", url, lat, lon, name, country);

                lat_s = atof(lat);
                lon_s = atof(lon);

                distance = calcDistance(lat_c, lon_c, lat_s, lon_s);

                for (i = 0; i < NEAREST_SERVERS_NUM; i++)
                {
                    if (nearest_servers[i].url[0] == '\0')
                    {
                        strncpy(nearest_servers[i].url, url, sizeof(url));
                        nearest_servers[i].latitude = lat_s;
                        nearest_servers[i].longitude = lon_s;
                        strncpy(nearest_servers[i].name, name, sizeof(name));
                        strncpy(nearest_servers[i].country, country, sizeof(country));
                        nearest_servers[i].distance = distance;
                        break;
                    }
                    else
                    {
                        if (distance <= nearest_servers[i].distance)
                        {
                            memset(&nearest_servers[NEAREST_SERVERS_NUM - 1], 0, sizeof(server_data_t));
                            for (j = NEAREST_SERVERS_NUM - 1; j > i; j--)
                            {
                                strncpy(nearest_servers[j].url, nearest_servers[j - 1].url, sizeof(nearest_servers[j - 1].url));
                                nearest_servers[j].latitude = nearest_servers[j - 1].latitude;
                                nearest_servers[j].longitude = nearest_servers[j - 1].longitude;
                                strncpy(nearest_servers[j].name, nearest_servers[j - 1].name, sizeof(nearest_servers[j - 1].name));
                                strncpy(nearest_servers[j].country, nearest_servers[j - 1].country, sizeof(nearest_servers[j - 1].country));
                                nearest_servers[j].distance = nearest_servers[j - 1].distance;
                            }
                            strncpy(nearest_servers[i].url, url, sizeof(url));
                            nearest_servers[i].latitude = lat_s;
                            nearest_servers[i].longitude = lon_s;
                            strncpy(nearest_servers[i].name, name, sizeof(name));
                            strncpy(nearest_servers[i].country, country, sizeof(country));
                            nearest_servers[i].distance = distance;
                            break;
                        }
                    }
                }
                count++;
            }
        }
    }
    if (count > 0)
        return 1;
    else
        return 0;
}

int compare_latency(const void *a, const void *b)
{
    const server_data_t *server_a = (const server_data_t *)a;
    const server_data_t *server_b = (const server_data_t *)b;

    if (server_a->latency == -1 && server_b->latency == -1)
    {
        return 0;
    }
    else if (server_a->latency == -1)
    {
        return 1;
    }
    else if (server_b->latency == -1)
    {
        return -1;
    }
    else
    {
        return server_a->latency - server_b->latency;
    }
}

int get_best_server(server_data_t *nearest_servers)
{
    FILE *fp = NULL;
    int i = 0, latency, best_index = -1;
    char latency_name[16] = "latency.txt";
    char latency_file_string[16] = "test=test";
    char latency_url[NEAREST_SERVERS_NUM][128], latency_request_url[128];
    char url[128], buf[128], filePath[64] = {0}, line[64] = {0};
    struct timeval tv1, tv2;
    struct sockaddr_in servinfo;

    sprintf(filePath, "%s%s", FILE_DIRECTORY_PATH, latency_name);
    
    // Generate request url for latency
    for (i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        char *ptr = NULL;
        memset(latency_url[i], 0, sizeof(latency_url[i]));
        strncpy(url, nearest_servers[i].url, sizeof(nearest_servers[i].url));

        ptr = strtok(url, "/");
        while (ptr != NULL)
        {
            memset(buf, 0, sizeof(buf));
            strncpy(buf, ptr, strlen(ptr));

            // Change file name to "latency.txt"
            if (strstr(buf, "upload.") != NULL)
            {
                strcat(latency_url[i], latency_name);
            }
            else
            {
                strcat(latency_url[i], buf);
                strcat(latency_url[i], "/");
            }

            if (strstr(buf, "http:"))
                strcat(latency_url[i], "/");

            ptr = strtok(NULL, "/");
        }
        
        // Get domain name
        sscanf(latency_url[i], "http://%[^/]", nearest_servers[i].domain_name);

        // Get request url
        memset(latency_request_url, 0, sizeof(latency_request_url));
        if ((ptr = strstr(latency_url[i], nearest_servers[i].domain_name)) != NULL)
        {
            ptr += strlen(nearest_servers[i].domain_name);
            strncpy(latency_request_url, ptr, strlen(ptr));
        }
        
        if (get_ipv4_addr(nearest_servers[i].domain_name, &servinfo))
        {
            memcpy(&nearest_servers[i].servinfo, &servinfo, sizeof(struct sockaddr_in));

            // Get latency time
            gettimeofday(&tv1, NULL);
            get_http_file(&servinfo, nearest_servers[i].domain_name, latency_request_url, latency_name);
            gettimeofday(&tv2, NULL);
        }
        
        if ((fp = fopen(filePath, "r")) != NULL)
        {
            fgets(line, sizeof(line), fp);
            if (!strncmp(line, latency_file_string, strlen(latency_file_string)))
                nearest_servers[i].latency = (tv2.tv_sec - tv1.tv_sec) * 1000000 + tv2.tv_usec - tv1.tv_usec;
            else
                nearest_servers[i].latency = -1;

            fclose(fp);
            unlink(filePath);
        }
        else
        {
            nearest_servers[i].latency = -1;
        }
    }
    
    // Sort servers by latency
    qsort(nearest_servers, NEAREST_SERVERS_NUM, sizeof(server_data_t), compare_latency);

    // Select low latency server
    for (i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        if (nearest_servers[i].latency != -1)
        {
            best_index = i;
            break;
        }
    }
    
    return best_index;
}

int get_best_server_https(server_data_t *nearest_servers)
{
    FILE *fp = NULL;
    int i = 0, latency, best_index = -1;
    char latency_name[16] = "latency.txt";
    char latency_file_string[16] = "test=test";
    char latency_url[NEAREST_SERVERS_NUM][128], latency_request_url[128];
    char url[128], buf[128], filePath[64] = {0}, line[64] = {0};
    struct timeval tv1, tv2;
    struct sockaddr_in servinfo;

    sprintf(filePath, "%s%s", FILE_DIRECTORY_PATH, latency_name);
    
    // Generate request url for latency
    for (i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        char *ptr = NULL;
        memset(latency_url[i], 0, sizeof(latency_url[i]));
        strncpy(url, nearest_servers[i].url, sizeof(nearest_servers[i].url));

        ptr = strtok(url, "/");
        while (ptr != NULL)
        {
            memset(buf, 0, sizeof(buf));
            strncpy(buf, ptr, strlen(ptr));

            // Change file name to "latency.txt"
            if (strstr(buf, "upload.") != NULL)
            {
                strcat(latency_url[i], latency_name);
            }
            else
            {
                strcat(latency_url[i], buf);
                strcat(latency_url[i], "/");
            }

            if (strstr(buf, "http:"))
                strcat(latency_url[i], "/");

            ptr = strtok(NULL, "/");
        }
        
        // Get domain name
        sscanf(latency_url[i], "http://%[^/]", nearest_servers[i].domain_name);

        // Get request url
        memset(latency_request_url, 0, sizeof(latency_request_url));
        if ((ptr = strstr(latency_url[i], nearest_servers[i].domain_name)) != NULL)
        {
            ptr += strlen(nearest_servers[i].domain_name);
            strncpy(latency_request_url, ptr, strlen(ptr));
        }
        
        if (get_ipv4_addr_https(nearest_servers[i].domain_name, &servinfo))
        {
            memcpy(&nearest_servers[i].servinfo, &servinfo, sizeof(struct sockaddr_in));

            // Get latency time
            gettimeofday(&tv1, NULL);
            get_https_file(&servinfo, nearest_servers[i].domain_name, latency_request_url, latency_name);
            gettimeofday(&tv2, NULL);
        }
        
        if ((fp = fopen(filePath, "r")) != NULL)
        {
            fgets(line, sizeof(line), fp);
            if (!strncmp(line, latency_file_string, strlen(latency_file_string)))
                nearest_servers[i].latency = (tv2.tv_sec - tv1.tv_sec) * 1000000 + tv2.tv_usec - tv1.tv_usec;
            else
                nearest_servers[i].latency = -1;

            fclose(fp);
            unlink(filePath);
        }
        else
        {
            nearest_servers[i].latency = -1;
        }
    }
    
    // Sort servers by latency
    qsort(nearest_servers, NEAREST_SERVERS_NUM, sizeof(server_data_t), compare_latency);

    // Select low latency server
    for (i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        if (nearest_servers[i].latency != -1)
        {
            best_index = i;
            break;
        }
    }
    
    return best_index;
}

int check_server(server_data_t server_data)
{
    int fd;
    char sbuf[512] = {0};
    struct timeval tv;
    fd_set fdSet;
    
    if ((fd = socket(server_data.servinfo.sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        return 1;
    }

    if (connect(fd, (struct sockaddr *)&server_data.servinfo, sizeof(struct sockaddr)) == -1)
    {
        perror("Socket connect error!\n");
        return 1;
    }

    sprintf(sbuf,
            "GET /%s HTTP/1.0\r\n"
            "Host: %s\r\n"
            "User-Agent: status\r\n"
            "Accept: */*\r\n\r\n",
            server_data.url, server_data.domain_name);

    if (send(fd, sbuf, strlen(sbuf), 0) != strlen(sbuf))
    {
        perror("Can't send data to server\n");
        return 1;
    }

    return 0;
}

int check_server_https(server_data_t server_data)
{
    int fd;
    char sbuf[512] = {0};
    struct timeval tv;
    fd_set fdSet;
    SSL_CTX *ctx;
    SSL *ssl;

    // Initialize OpenSSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    // Create SSL context
    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "SSL_CTX_new() failed\n");
        return 1;
    }

    if ((fd = socket(server_data.servinfo.sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        SSL_CTX_free(ctx);
        return 1;
    }

    // Connect to server
    if (connect(fd, (struct sockaddr *)&server_data.servinfo, sizeof(struct sockaddr)) == -1)
    {
        perror("Socket connect error!\n");
        SSL_CTX_free(ctx);
        close(fd);
        return 1;
    }

    // Create SSL connection
    ssl = SSL_new(ctx);
    if (!ssl) {
        fprintf(stderr, "SSL_new() failed\n");
        SSL_CTX_free(ctx);
        close(fd);
        return 1;
    }

    // Attach SSL to socket
    SSL_set_fd(ssl, fd);

    // Perform SSL handshake
    if (SSL_connect(ssl) != 1) {
        fprintf(stderr, "SSL_connect() failed\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(fd);
        return 1;
    }

    sprintf(sbuf,
            "GET /%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "User-Agent: status\r\n"
            "Accept: */*\r\n"
            "Connection: close\r\n\r\n",
            server_data.url, server_data.domain_name);

    if (SSL_write(ssl, sbuf, strlen(sbuf)) != strlen(sbuf))
    {
        perror("Can't send data to server\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(fd);
        return 1;
    }

    // Wait for response
    FD_ZERO(&fdSet);
    FD_SET(fd, &fdSet);
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    if (select(fd + 1, &fdSet, NULL, NULL, &tv) == -1)
    {
        perror("Select error!\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(fd);
        return 1;
    }

    if (FD_ISSET(fd, &fdSet))
    {
        char buf[1024];
        int n = SSL_read(ssl, buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = '\0';
            printf("%s", buf);
        }
    }

    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(fd);

    return 0;
}

