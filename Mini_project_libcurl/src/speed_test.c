#include "speed_test.h"

// File for upload test
const char *file_path = "upload.txt";
int test_time = 3;

int g_timeout_threshold = 1;

void set_timeout_threshold(int timeout_threshold)
{
    g_timeout_threshold = timeout_threshold;
}

void set_test_time(int time)
{
    test_time = time;
}

// Thread function to test internet upload speed
void *test_upload_speed_thread(void *arg)
{
    ThreadData *thread_data = (ThreadData *)arg;
    pthread_mutex_t *mutex = thread_data->mutex;
    CURL *curl;
    CURLcode res;
    double upload_speed = 0.0;
    time_t start_time = 0;
    double speed = 0;

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
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, g_timeout_threshold);

        // Perform the upload for the specified duration
        start_time = time(NULL);
        do
        {
            res = curl_easy_perform(curl);
            if (res == CURLE_OK)
            {
                curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed);
                upload_speed += speed;
            }
            else
            {
                // fprintf(stderr, "Upload failed: %s\n", curl_easy_strerror(res));
            }
        } while (time(NULL) - start_time < test_time);

        // Clean up
        curl_easy_cleanup(curl);
    }

    // Cleanup global curl
    curl_global_cleanup();

    // Calculate average upload speed
    pthread_mutex_lock(mutex);
    thread_data->avg_speed = upload_speed / test_time;
    pthread_mutex_unlock(mutex);

    pthread_exit(NULL);
}

// Function to test internet upload speed using multiple threads
double test_upload_speed(const char *url, const char *file_path, int num_thread)
{
    pthread_t threads[num_thread];
    ThreadData thread_data[num_thread];
    double total_upload_speed = 0.0;
    double avg_speed = 0;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < num_thread; i++)
    {
        thread_data[i].url = url;
        thread_data[i].file_path = file_path;
        thread_data[i].avg_speed = 0.0;
        thread_data[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, test_upload_speed_thread, (void *)&thread_data[i]);
    }

    for (int i = 0; i < num_thread; i++)
    {
        pthread_join(threads[i], NULL);
        total_upload_speed += thread_data[i].avg_speed;
    }

    avg_speed = total_upload_speed / num_thread;
    printf("Average Upload Speed: %.2f Mb/sec\n", avg_speed / 125000);

    return avg_speed;
}

// Thread function to test internet upload speed
void *test_download_speed_thread(void *arg)
{
    ThreadData *thread_data = (ThreadData *)arg;

    pthread_mutex_t *mutex = thread_data->mutex;

    CURL *curl;
    CURLcode res;
    double download_speed = 0.0;
    time_t start_time = 0;
    double speed = 0;

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
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, g_timeout_threshold);

        // Perform the download
        res = curl_easy_perform(curl);

        // Perform the upload for the specified duration
        start_time = time(NULL);
        do
        {
            res = curl_easy_perform(curl);
            if (res == CURLE_OK)
            {
                curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &download_speed);
                download_speed += speed;
            }
            else
            {
                // fprintf(stderr, "Upload failed: %s\n", curl_easy_strerror(res));
            }
        } while (time(NULL) - start_time < test_time);

        // Clean up
        curl_easy_cleanup(curl);
    }

    // Cleanup global curl
    curl_global_cleanup();

    // Calculate average upload speed
    pthread_mutex_lock(mutex);
    thread_data->avg_speed = download_speed / test_time;
    pthread_mutex_unlock(mutex);

    pthread_exit(NULL);
}

// Function to test internet upload speed using multiple threads
double test_download_speed(const char *url, int num_thread)
{
    pthread_t threads[num_thread];
    ThreadData thread_data[num_thread];
    double total_download_speed = 0.0;
    double avg_speed = 0;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < num_thread; i++)
    {
        thread_data[i].url = url;
        thread_data[i].file_path = NULL;
        thread_data[i].avg_speed = 0.0;
        thread_data[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, test_download_speed_thread, (void *)&thread_data[i]);
    }

    for (int i = 0; i < num_thread; i++)
    {
        pthread_join(threads[i], NULL);
        total_download_speed += thread_data[i].avg_speed;
    }

    avg_speed = total_download_speed / num_thread;
    printf("Average Download Speed: %.2f Mb/sec\n", avg_speed / 125000);

    return avg_speed;
}

void run_speed_test(int num_thread, bool https, bool auto_pick_server)
{
    int count = 0;
    int choose = 0;
    bool speed_test_check = false;
    double upload_speed = 0;
    double download_speed = 0;
    int server_num = 0;

    // Find all the server nearby
    find_best_server();

    // Parse server informations
    server_num = parse_server_data(https);

    // Format server url
    format_url(server_num);

    // Calculate server latency
    get_servers_latency(server_num);

    // Sort server by latency
    sort_servers_by_latency(server_num);

    print_servers(server_num);

    if (auto_pick_server)
    {
        count = 0;
        while (count < server_num)
        {
            printf("Testing speed from lowest latency server: %s\n", get_server_url(count));

            upload_speed = test_upload_speed(get_server_url(count), file_path, num_thread);
            if (upload_speed <= 0)
            {
                printf("Upload test failed. Trying next server ...\n");
                count++;
                if (count >= server_num)
                {
                    printf("No server available\n");
                    break;
                }
                continue;
            }

            download_speed = test_download_speed(get_server_url(count), num_thread);
            if (download_speed <= 0)
            {
                printf("Download speed test failed. Trying next server ...\n");
                count++;
                if (count >= server_num)
                {
                    printf("No server available\n");
                    break;
                }
                continue;
            }

            break;
        }
    }
    else
    {
        while (1)
        {
            printf("Choose server to test: ");
            get_input_int(&choose, 0, server_num - 1);
            printf("Testing speed from: %s\n", get_server_url(choose));
            
            upload_speed = test_upload_speed(get_server_url(choose), file_path, num_thread);
            if (upload_speed <= 0)
            {
                printf("Upload test failed. Please choose a diffrent server.\n");
                continue;
            }

            download_speed = test_download_speed(get_server_url(count), num_thread);
            if (download_speed <= 0)
            {
                printf("Download test failed. Please choose a diffrent server.\n");
                continue;
            }

            break;
        }
    }
}