#include "servers.h"

Server servers[MAX_SERVERS];

// Function to discard response data
size_t discard_response(void *ptr, size_t size, size_t nmemb, void *data)
{
    // Discard the received data
    return size * nmemb;
}

char *get_server_url(int index)
{
    return servers[index].url;
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

int parse_server_data(bool https)
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
        if (!https)
        {
            if (servers[i].https_functional != 1)
            {
                continue;
            }
        }

        token = strtok(NULL, "!");

        if (i + 1 > MAX_SERVERS)
        {
            break;
        }

        i++;
    }

    return i;
}

void format_url(int server_num)
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

// Function to calculate server latency using libcurl
float measure_latency(Server* server)
{
    // Initialize libcurl
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "Failed to initialize libcurl\n");
        return -1.0f;
    }

    // Set the server URL
    curl_easy_setopt(curl, CURLOPT_URL, server->url);

    // Perform a HEAD request to measure latency
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

    // Set the write callback function to discard received data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_response);

    // Set timeout option
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_THRESHOLD);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        //fprintf(stderr, "Failed to perform the request: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return -1.0f;
    }

    // Get the total time taken for the request
    double total_time;
    res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "Failed to get total time: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return -1.0f;
    }

    // Cleanup libcurl
    curl_easy_cleanup(curl);

    // Return the measured latency
    return (float)total_time;
}

void get_servers_latency(int server_num)
{
    printf("Calculating server latency ...\n");

    for (int i = 0; i < server_num; i++)
    {
        float latency = measure_latency(&servers[i]);
        servers[i].latency = latency;
    }
}

void sort_servers_by_latency(int server_num)
{
    int i = 0;
    int j = 0;
    float latency_1 = 0;
    float latency_2 = 0;
    Server temp;

    for (i = 0; i < server_num - 1; i++)
    {
        for (j = 0; j < server_num - i - 1; j++)
        {
            latency_1 = servers[j].latency;
            latency_2 = servers[j + 1].latency;

            if (latency_1 == -1)
            {
                latency_1 = 9999999999;
            }
            if (latency_2 == -1)
            {
                latency_2 = 9999999999;
            }

            if (latency_1 > latency_2)
            {
                // Swap servers[j] and servers[j+1]
                memcpy(&temp, &servers[j], sizeof(Server));
                memcpy(&servers[j], &servers[j + 1], sizeof(Server));
                memcpy(&servers[j + 1], &temp, sizeof(Server));
            }
        }
    }
}

char *https_functional_to_string(int index)
{
    if (servers[index].https_functional == 1)
    {
        return "Yes";
    }
    else
    {
        return "No";
    }
}

void print_servers(int server_num)
{
    printf("=========================================================================================================================================================================\n");
    printf("%-5s %-60s %-15s %-15s %-15s %-15s %-15s %-15s\n", "ID", "URL", "Latitude", "Longitude", "HTTPS", "Country", "Distance", "Latency (ms)");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < server_num; i++)
    {
        printf("%-5i %-60s %-15s %-15s %-15s %-15s %-15i %-15.2f\n", i,
               servers[i].url,
               servers[i].lat,
               servers[i].lon,
               https_functional_to_string(i),
               servers[i].country,
               servers[i].distance,
               servers[i].latency);
    }
    printf("=========================================================================================================================================================================\n");
}