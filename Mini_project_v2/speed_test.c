#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

// Speedtest by Ookla API URL
#define SPEEDTEST_API_URL "https://www.speedtest.net/api/js/servers"
#define MAX_SERVERS 10

#define TIMEOUT_THRESHOLD 3

struct ServerInfo
{
    char name[256];
    char url[256];
    double latency;
    double distance;
    char country[256];
};

// Function to discard response data
size_t discard_response(void *ptr, size_t size, size_t nmemb, void *data) {
    // Discard the received data
    return size * nmemb;
}

// Function to test internet upload speed
double test_upload_speed(const char *url, const char *file_path) {
    CURL *curl;
    CURLcode res;
    double upload_speed = 0.0;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the URL to upload to
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the file to upload
        curl_easy_setopt(curl, CURLOPT_READDATA, fopen(file_path, "rb"));

        // Discard response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_response);

        // Set timeout option
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_THRESHOLD);

        // Perform the upload
        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            // Get upload speed
            curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &upload_speed);
            printf("Upload Speed: %.2f bytes/sec\n", upload_speed);
        } else {
            fprintf(stderr, "Upload failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    // Cleanup global curl
    curl_global_cleanup();

    return upload_speed;
}

// Function to test internet download speed
double test_download_speed(const char *url)
{
    CURL *curl;
    CURLcode res;
    double download_speed = 0.0;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        // Set the URL to download from
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Discard response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_response);

        // Set timeout option
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_THRESHOLD);

        // Perform the download
        res = curl_easy_perform(curl);

        if (res == CURLE_OK)
        {
            // Get download speed
            curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &download_speed);
            printf("Download Speed: %.2f bytes/sec\n", download_speed);
        }
        else
        {
            fprintf(stderr, "Download failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    // Cleanup global curl
    curl_global_cleanup();

    return download_speed;
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
    const char *upload_url = "http://speedtesthni2.viettel.vn";
    const char *download_url = "http://speedtesthni2.viettel.vn";
    const char *file_path = "upload.txt";

    // Find the best server nearby
    find_best_server();

    double upload_speed = test_upload_speed(upload_url, file_path);

    if (upload_speed > 0.0)
    {
        printf("Upload speed test completed.\n");
    }
    else
    {
        printf("Upload speed test failed.\n");
    }

    double download_speed = test_download_speed(download_url);

    if (download_speed > 0.0)
    {
        printf("Download speed test completed.\n");
    }
    else
    {
        printf("Download speed test failed.\n");
    }

    return 0;
}
