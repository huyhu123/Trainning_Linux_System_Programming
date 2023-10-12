#include "speed_test_download_upload.h"

static pthread_mutex_t pthread_mutex = PTHREAD_MUTEX_INITIALIZER;
float start_dl_time, stop_dl_time, start_ul_time, stop_ul_time;
long int total_dl_size = 0, total_ul_size = 0;

thread_t thread[MAX_THREAD_NUMBER];

int thread_all_stop = 0;

void stop_all_thread()
{
    thread_all_stop = 0;
}

float get_uptime(void)
{
    FILE *fp;
    float uptime, idle_time;

    if ((fp = fopen("/proc/uptime", "r")) != NULL)
    {
        fscanf(fp, "%f %f\n", &uptime, &idle_time);
        fclose(fp);
        return uptime;
    }
    return -1;
}

void *calculate_ul_speed_thread()
{
    double ul_speed = 0.0, duration = 0;
    while (1)
    {
        stop_ul_time = get_uptime();
        duration = stop_ul_time - start_ul_time;
        // ul_speed = (double)total_ul_size/1024/1024/duration*8;
        ul_speed = (double)total_ul_size / 1000 / 1000 / duration * 8;
        if (duration > 0)
        {
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bUpload speed: %0.2lf Mbps", ul_speed);
            fflush(stdout);
        }
        usleep(500000);

        if (thread_all_stop)
        {
            stop_ul_time = get_uptime();
            duration = stop_ul_time - start_ul_time;
            // ul_speed = (double)total_ul_size/1024/1024/duration*8;
            ul_speed = (double)total_ul_size / 1000 / 1000 / duration * 8;
            if (duration)
            {
                printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bUpload speed: %0.2lf Mbps", ul_speed);
                fflush(stdout);
            }
            break;
        }
    }
    return NULL;
}

void *calculate_dl_speed_thread()
{
    double dl_speed = 0.0, duration = 0;
    while (1)
    {
        stop_dl_time = get_uptime();
        duration = stop_dl_time - start_dl_time;
        // dl_speed = (double)total_dl_size/1024/1024/duration*8;
        dl_speed = (double)total_dl_size / 1000 / 1000 / duration * 8;
        if (duration > 0)
        {
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bDownload speed: %0.2lf Mbps", dl_speed);
            fflush(stdout);
        }
        usleep(500000);

        if (thread_all_stop)
        {
            stop_dl_time = get_uptime();
            duration = stop_dl_time - start_dl_time;
            dl_speed = (double)total_dl_size / 1000 / 1000 / duration * 8;
            if (duration > 0)
            {
                printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bDownload speed: %0.2lf Mbps", dl_speed);
                fflush(stdout);
            }
            break;
        }
    }
    return NULL;
}

void *download_thread(void *arg)
{
    thread_t *t_arg = arg;
    int i = t_arg->thread_index;

    int fd;
    char sbuf[512] = {0}, rbuf[DL_BUFFER_SIZE];
    struct timeval tv;
    fd_set fdSet;

    if ((fd = socket(thread[i].servinfo.sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        goto err;
    }

    if (connect(fd, (struct sockaddr *)&thread[i].servinfo, sizeof(struct sockaddr)) == -1)
    {
        perror("Socket connect error!\n");
        goto err;
    }

    sprintf(sbuf,
            "GET /%s HTTP/1.0\r\n"
            "Host: %s\r\n"
            "User-Agent: status\r\n"
            "Accept: */*\r\n\r\n",
            thread[i].request_url, thread[i].domain_name);

    if (send(fd, sbuf, str_len(sbuf), 0) != str_len(sbuf))
    {
        perror("Can't send data to server\n");
        goto err;
    }

    while (1)
    {
        FD_ZERO(&fdSet);
        FD_SET(fd, &fdSet);

        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int status = select(fd + 1, &fdSet, NULL, NULL, &tv);

        int recv_byte = recv(fd, rbuf, sizeof(rbuf), 0);
        if (status > 0 && FD_ISSET(fd, &fdSet))
        {
            if (recv_byte < 0)
            {
                printf("Can't receive data!\n");
                break;
            }
            else if (recv_byte == 0)
            {
                break;
            }
            else
            {
                pthread_mutex_lock(&pthread_mutex);
                total_dl_size += recv_byte;
                pthread_mutex_unlock(&pthread_mutex);
            }

            if (thread_all_stop)
                break;
        }
    }

err:
    if (fd)
        close(fd);
    // thread_all_stop = 1;
    thread[i].running = 0;
    return NULL;
}

int speedtest_download(server_data_t *nearest_server, int thread_num)
{
    const char download_filename[64] = "random3500x3500.jpg"; // 23MB
    char url[128] = {0}, request_url[128] = {0}, dummy[128] = {0}, buf[128];
    char *ptr = NULL;
    int i;
    clock_t start = 0;
    clock_t end = 0;
    double cpu_time_used = 0;

    sscanf(nearest_server->url, "http://%[^/]/%s", dummy, request_url);
    strncpy(url, request_url, sizeof(request_url));
    memset(request_url, 0, sizeof(request_url));

    ptr = strtok(url, "/");
    while (ptr != NULL)
    {
        memset(buf, 0, sizeof(buf));
        strncpy(buf, ptr, str_len(ptr));

        // Change file name
        if (strstr(buf, "upload.") != NULL)
        {
            strcat(request_url, download_filename);
        }
        else
        {
            strcat(request_url, buf);
            strcat(request_url, "/");
        }
        ptr = strtok(NULL, "/");
    }

    start_dl_time = get_uptime();

    start = clock();
    while (1)
    {
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        // printf("%f\n", cpu_time_used);
        if (cpu_time_used > SPEEDTEST_DURATION)
        {
            thread_all_stop = 1;
        }

        for (i = 0; i < thread_num; i++)
        {
            memcpy(&thread[i].servinfo, &nearest_server->servinfo, sizeof(struct sockaddr_in));
            memcpy(&thread[i].domain_name, &nearest_server->domain_name, sizeof(nearest_server->domain_name));
            memcpy(&thread[i].request_url, request_url, sizeof(request_url));
            if (thread[i].running == 0)
            {
                thread[i].thread_index = i;
                thread[i].running = 1;
                pthread_create(&thread[i].tid, NULL, download_thread, &thread[i]);
            }
        }
        if (thread_all_stop == 1)
            break;
    }
    return 1;
}

void *upload_thread(void *arg)
{
    int fd;
    char data[UL_BUFFER_SIZE], sbuf[512];
    int i, j, size = 0;
    struct timeval tv;
    fd_set fdSet;

    thread_t *t_arg = arg;
    i = t_arg->thread_index;

    memset(data, 0, sizeof(char) * UL_BUFFER_SIZE);

    if ((fd = socket(thread[i].servinfo.sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        goto err;
    }

    if (connect(fd, (struct sockaddr *)&thread[i].servinfo, sizeof(struct sockaddr)) == -1)
    {
        printf("Socket connect error!\n");
        goto err;
    }

    sprintf(sbuf,
            "POST /%s HTTP/1.0\r\n"
            "Content-type: application/x-www-form-urlencoded\r\n"
            "Host: %s\r\n"
            "Content-Length: %ld\r\n\r\n",
            thread[i].request_url, thread[i].domain_name, sizeof(data) * UL_BUFFER_TIMES);

    if ((size = send(fd, sbuf, str_len(sbuf), 0)) != str_len(sbuf))
    {
        printf("Can't send header to server\n");
        goto err;
    }

    pthread_mutex_lock(&pthread_mutex);
    total_ul_size += size;
    pthread_mutex_unlock(&pthread_mutex);

    for (j = 0; j < UL_BUFFER_TIMES; j++)
    {
        if ((size = send(fd, data, sizeof(data), 0)) != sizeof(data))
        {
            printf("Can't send data to server\n");
            goto err;
        }
        pthread_mutex_lock(&pthread_mutex);
        total_ul_size += size;
        pthread_mutex_unlock(&pthread_mutex);
        if (thread_all_stop)
            goto err;
    }

    while (1)
    {
        FD_ZERO(&fdSet);
        FD_SET(fd, &fdSet);

        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int status = select(fd + 1, &fdSet, NULL, NULL, &tv);

        int recv_byte = recv(fd, sbuf, sizeof(sbuf), 0);
        if (status > 0 && FD_ISSET(fd, &fdSet))
        {
            if (recv_byte < 0)
            {
                printf("Can't receive data!\n");
                break;
            }
            else if (recv_byte == 0)
            {
                break;
            }
        }
    }
err:
    if (fd)
        close(fd);
    // thread_all_stop = 1;
    thread[i].running = 0;
    return NULL;
}

int speedtest_upload(server_data_t *nearest_server, int thread_num)
{
    int i;
    clock_t start = 0;
    clock_t end = 0;
    double cpu_time_used = 0;
    char dummy[128] = {0}, request_url[128] = {0};
    sscanf(nearest_server->url, "http://%[^/]/%s", dummy, request_url);

    start = clock();
    start_ul_time = get_uptime();
    while (1)
    {
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (cpu_time_used > SPEEDTEST_DURATION)
        {
            thread_all_stop = 1;
        }

        for (i = 0; i < thread_num; i++)
        {
            memcpy(&thread[i].servinfo, &nearest_server->servinfo, sizeof(struct sockaddr_in));
            memcpy(&thread[i].domain_name, &nearest_server->domain_name, sizeof(nearest_server->domain_name));
            memcpy(&thread[i].request_url, request_url, sizeof(request_url));
            if (thread[i].running == 0)
            {
                thread[i].thread_index = i;
                thread[i].running = 1;
                pthread_create(&thread[i].tid, NULL, upload_thread, &thread[i]);
            }
        }
        if (thread_all_stop)
            break;
    }
    return 1;
}