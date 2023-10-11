#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// Todo
// Mutex

// Duration to mesure speed
#define DURATION_SECONDS 3

// Speedtest by Ookla API URL
#define SPEEDTEST_API_URL "https://www.speedtest.net/api/js/servers"

// Todo
#define MAX_SERVERS 100

#define MAX_LENGTH 500
#define NUM_THREADS 10

#define TIMEOUT_THRESHOLD 3

typedef struct
{
    char url[100];
    char lat[20];
    char lon[20];
    int distance;
    char name[100];
    char country[100];
    char cc[3];
    char sponsor[100];
    char id[10];
    int preferred;
    int https_functional;
    char host[100];
    int force_ping_select;
} Server;

typedef struct
{
    const char *url;
    const char *file_path;
    double avg_speed;
    pthread_mutex_t *mutex;
} ThreadData;

Server servers[MAX_SERVERS];

// Function to discard response data
size_t discard_response(void *ptr, size_t size, size_t nmemb, void *data)
{
    // Discard the received data
    return size * nmemb;
}

// Thread function to test internet upload speed
void *test_upload_speed_thread(void *arg)
{
    ThreadData *thread_data = (ThreadData *)arg;

    pthread_mutex_t *mutex = thread_data->mutex;

    CURL *curl;
    CURLcode res;
    double upload_speed = 0.0;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        // Set the URL to upload to
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, thread_data->url);

        // Set the file to upload
        curl_easy_setopt(curl, CURLOPT_READDATA, fopen(thread_data->file_path, "rb"));

        // Discard response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_response);

        // Set the timeout for the upload connection
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_THRESHOLD);

        // Perform the upload for the specified duration
        time_t start_time = time(NULL);
        do
        {
            res = curl_easy_perform(curl);
            if (res == CURLE_OK)
            {
                double speed;
                curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed);
                upload_speed += speed;
            }
            else
            {
                // fprintf(stderr, "Upload failed: %s\n", curl_easy_strerror(res));
            }
        } while (time(NULL) - start_time < DURATION_SECONDS);

        // Clean up
        curl_easy_cleanup(curl);
    }

    // Cleanup global curl
    curl_global_cleanup();

    // Calculate average upload speed
    pthread_mutex_lock(mutex);
    thread_data->avg_speed = upload_speed / DURATION_SECONDS;
    pthread_mutex_unlock(mutex);

    pthread_exit(NULL);
}

// Function to test internet upload speed using multiple threads
double test_upload_speed(const char *url, const char *file_path)
{
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    double total_upload_speed = 0.0;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].url = url;
        thread_data[i].file_path = file_path;
        thread_data[i].avg_speed = 0.0;
        thread_data[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, test_upload_speed_thread, (void *)&thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
        total_upload_speed += thread_data[i].avg_speed;
    }

    double avg_speed = total_upload_speed / NUM_THREADS;
    printf("Average Upload Speed: %.2f Mbs/sec\n", avg_speed / 1000);

    return avg_speed;
}

void replace(char str[], char sub[], char nstr[])
{
    int strLen, subLen, nstrLen;
    int i = 0, j, k;
    int flag = 0, start, end;

    strLen = strlen(str);
    subLen = strlen(sub);
    nstrLen = strlen(nstr);

    for (i = 0; i < strLen; i++)
    {
        flag = 0;
        start = i;
        for (j = 0; str[i] == sub[j]; j++, i++)
            if (j == subLen - 1)
                flag = 1;
        end = i;

        if (flag == 0)
            i -= j;
        else
        {
            for (j = start; j < end; j++)
            {
                for (k = start; k < strLen; k++)
                    str[k] = str[k + 1];
                strLen--;
                i--;
            }
            for (j = start; j < start + nstrLen; j++)
            {
                for (k = strLen; k >= j; k--)
                    str[k + 1] = str[k];
                str[j] = nstr[j - start];
                strLen++;
                i++;
            }
        }
    }
}

int parse_server_data()
{
    FILE *file = fopen("output.txt", "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 0;
    }

    // Read the contents of the file
    char line[10000];
    fgets(line, sizeof(line), file);

    replace(line, "},{", "!");

    int i = 0;
    char *token = strtok(line, "!");

    while (token != NULL)
    {
        if (i == 0)
        {
            memmove(&token[i], &token[i + 1], strlen(token) - i);
            memmove(&token[i], &token[i + 1], strlen(token) - i);
        }

        sscanf(token,
               "\"url\":\"%[^\"]\",\"lat\":\"%[^\"]\",\"lon\":\"%[^\"]\",\"distance\":%d,\"name\":\"%[^\"]\",\"country\":\"%[^\"]\",\"cc\":\"%[^\"]\",\"sponsor\":\"%[^\"]\",\"id\":\"%[^\"]\",\"preferred\":%d,\"https_functional\":%d,\"host\":\"%[^\"]\",\"force_ping_select\":%d",
               servers[i].url, servers[i].lat, servers[i].lon, &servers[i].distance, servers[i].name,
               servers[i].country, servers[i].cc, servers[i].sponsor, servers[i].id, &servers[i].preferred,
               &servers[i].https_functional, servers[i].host, &servers[i].force_ping_select);

        // printf("%s\n\n", token);

        token = strtok(NULL, "!");
        i++;
    }

    return i;
}

void fix_url(int server_num)
{
    for (int i = 0; i < server_num; i++)
    {
        replace(servers[i].url, ":8", "!");
        replace(servers[i].url, "\\/\\/", "//");
        char *token = strtok(servers[i].url, "!");
        strcpy(servers[i].url, token);
        strcat(servers[i].url, "\0");
        // printf("%s\n", servers[i].url);
    }
}

// Thread function to test internet upload speed
void *test_download_speed_thread(void *arg)
{
    ThreadData *thread_data = (ThreadData *)arg;

    pthread_mutex_t *mutex = thread_data->mutex;

    CURL *curl;
    CURLcode res;
    double download_speed = 0.0;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        // Set the URL to download from
        curl_easy_setopt(curl, CURLOPT_URL, thread_data->url);

        // Discard response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_response);

        // Set timeout option
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_THRESHOLD);

        // Perform the download
        res = curl_easy_perform(curl);

        // Perform the upload for the specified duration
        time_t start_time = time(NULL);
        do
        {
            res = curl_easy_perform(curl);
            if (res == CURLE_OK)
            {
                double speed;
                curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &download_speed);
                download_speed += speed;
            }
            else
            {
                // fprintf(stderr, "Upload failed: %s\n", curl_easy_strerror(res));
            }
        } while (time(NULL) - start_time < DURATION_SECONDS);

        // Clean up
        curl_easy_cleanup(curl);
    }

    // Cleanup global curl
    curl_global_cleanup();

    // Calculate average upload speed
    pthread_mutex_lock(mutex);
    thread_data->avg_speed = download_speed / DURATION_SECONDS;
    pthread_mutex_unlock(mutex);

    pthread_exit(NULL);
}

// Function to test internet upload speed using multiple threads
double test_download_speed(const char *url)
{
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    double total_download_speed = 0.0;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].url = url;
        thread_data[i].file_path = NULL;
        thread_data[i].avg_speed = 0.0;
        thread_data[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, test_download_speed_thread, (void *)&thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
        total_download_speed += thread_data[i].avg_speed;
    }

    double avg_speed = total_download_speed / NUM_THREADS;
    printf("Average Download Speed: %.2f Mbs/sec\n", avg_speed / 1000);

    return avg_speed;
}

// Function to find the best server nearby
void find_best_server()
{
    // Redirect stdout to output.txt
    freopen("output.txt", "w", stdout);

    CURL *curl;
    CURLcode res;
    struct json_object *json_servers;
    struct json_object *json_server;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        // Set the URL of the server speed test API
        curl_easy_setopt(curl, CURLOPT_URL, SPEEDTEST_API_URL);

        // Perform the request
        res = curl_easy_perform(curl);

        // Clean up
        curl_easy_cleanup(curl);
    }

    // Cleanup global curl
    curl_global_cleanup();

    // Set stdout to default setting
    freopen("/dev/tty", "w", stdout);
}

int main()
{
    // Find all the server nearby
    find_best_server();

    int server_num = parse_server_data();
    fix_url(server_num);

    const char *file_path = "upload.txt";

    for (int i = 0; i < server_num; i++)
    {
        char *upload_url = servers[i].url;
        char *download_url = servers[i].url;
        printf("%s\n", servers[i].url);

        double upload_speed = test_upload_speed(upload_url, file_path);
        if (upload_speed <= 0)
        {
            printf("Upload speed test failed.\n");
        }

        double download_speed = test_download_speed(download_url);
        if (download_speed <= 0)
        {
            printf("Download speed test failed.\n");
        }
    }

    return 0;
}
