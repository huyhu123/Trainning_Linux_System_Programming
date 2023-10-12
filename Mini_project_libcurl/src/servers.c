#include "servers.h"

server_t servers[MAX_SERVERS];
server_t client_server;

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

void delete_server_by_index(int index)
{
    if (index < 0 || index >= MAX_SERVERS)
    {
        printf("Invalid index!");
        return;
    }

    // Shift elements to the left starting from the index
    for (int i = index; i < MAX_SERVERS - 1; i++)
    {
        strcpy(servers[i].url, servers[i + 1].url);
        strcpy(servers[i].lat, servers[i + 1].lat);
        strcpy(servers[i].lon, servers[i + 1].lon);
        servers[i].distance = servers[i + 1].distance;
        strcpy(servers[i].name, servers[i + 1].name);
        strcpy(servers[i].country, servers[i + 1].country);
        strcpy(servers[i].cc, servers[i + 1].cc);
        strcpy(servers[i].sponsor, servers[i + 1].sponsor);
        strcpy(servers[i].id, servers[i + 1].id);
        servers[i].preferred = servers[i + 1].preferred;
        servers[i].https_functional = servers[i + 1].https_functional;
        strcpy(servers[i].host, servers[i + 1].host);
        servers[i].force_ping_select = servers[i + 1].force_ping_select;
        servers[i].latency = servers[i + 1].latency;
    }

    // Clear the last element
    memset(&servers[MAX_SERVERS - 1], 0, sizeof(server_t));
}

// Haversine formula
double calc_distance(double lat1, double lon1, double lat2, double lon2)
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

void replace(char str[], char sub[], char nstr[])
{
    int str_len = 0;
    int sub_len = 0;
    int nstr_len = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int flag = 0;
    int start = 0;
    int end = 0;

    str_len = strlen(str);
    sub_len = strlen(sub);
    nstr_len = strlen(nstr);

    for (i = 0; i < str_len; i++)
    {
        flag = 0;
        start = i;
        for (j = 0; str[i] == sub[j]; j++, i++)
            if (j == sub_len - 1)
                flag = 1;
        end = i;

        if (flag == 0)
            i -= j;
        else
        {
            for (j = start; j < end; j++)
            {
                for (k = start; k < str_len; k++)
                    str[k] = str[k + 1];
                str_len--;
                i--;
            }
            for (j = start; j < start + nstr_len; j++)
            {
                for (k = str_len; k >= j; k--)
                    str[k + 1] = str[k];
                str[j] = nstr[j - start];
                str_len++;
                i++;
            }
        }
    }
}

// Function to find the best server nearby
void find_best_server()
{
    CURL *curl;
    CURLcode res;
    struct json_object *json_servers;
    struct json_object *json_server;

    // Redirect stdout to output.txt
    freopen("output.txt", "w", stdout);

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
    char line[FILE_LENGTH];
    int i = 0;
    char *token;
    int count = 0;

    FILE *file = fopen("output.txt", "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 0;
    }

    // get_ip_address_position(CONFIG_REQUEST_URL, &client_server);

    // Read the contents of the file
    fgets(line, sizeof(line), file);

    replace(line, "},{", "!");
    token = strtok(line, "!");
    while (token != NULL)
    {
        // Delete [{ at the start
        if (i == 0)
        {
            memmove(&token[i], &token[i + 1], strlen(token) - i);
            memmove(&token[i], &token[i + 1], strlen(token) - i);
        }

        // Parse server data
        sscanf(token,
               "\"url\":\"%[^\"]\",\"lat\":\"%[^\"]\",\"lon\":\"%[^\"]\",\"distance\":%d,\"name\":\"%[^\"]\",\"country\":\"%[^\"]\",\"cc\":\"%[^\"]\",\"sponsor\":\"%[^\"]\",\"id\":\"%[^\"]\",\"preferred\":%d,\"https_functional\":%d,\"host\":\"%[^\"]\",\"force_ping_select\":%d",
               servers[i].url, servers[i].lat, servers[i].lon, &servers[i].distance, servers[i].name,
               servers[i].country, servers[i].cc, servers[i].sponsor, servers[i].id, &servers[i].preferred,
               &servers[i].https_functional, servers[i].host, &servers[i].force_ping_select);

        token = strtok(NULL, "!");

        if (i + 1 > MAX_SERVERS)
        {
            break;
        }
        i++;
    }

    remove("output.txt");

    // Filter out http and https server
    while (count <= i)
    {
        if (https)
        {
            if (servers[count].url[4] != 's')
            {
                delete_server_by_index(count);
                count--;
                i--;
            }
        }
        else
        {
            if (servers[count].url[4] == 's')
            {
                delete_server_by_index(count);
                count--;
                i--;
            }
        }
        count++;
    }
    if (https)
    {
        i++;
    }

    if (i == 0)
    {
        printf("\nList server empty!!\n");
    }
    return i;
}

void format_url(int server_num)
{
    char *token;

    for (int i = 0; i < server_num; i++)
    {
        replace(servers[i].url, ":8", "!");
        replace(servers[i].url, "\\/\\/", "//");
        token = strtok(servers[i].url, "!");
        strcpy(servers[i].url, token);
        strcat(servers[i].url, "\0");
    }
}

// Function to calculate server latency using libcurl
float measure_latency(server_t *server)
{
    double total_time;

    // Initialize libcurl
    CURL *curl = curl_easy_init();
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
        // fprintf(stderr, "Failed to perform the request: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return -1.0f;
    }

    // Get the total time taken for the request
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
    float latency = 0;
    printf("Calculating server latency ...\n");

    for (int i = 0; i < server_num; i++)
    {
        latency = measure_latency(&servers[i]);
        servers[i].latency = latency;
    }
}

void sort_servers_by_latency(int server_num)
{
    int i = 0;
    int j = 0;
    float latency_1 = 0;
    float latency_2 = 0;
    server_t temp;

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
                memcpy(&temp, &servers[j], sizeof(server_t));
                memcpy(&servers[j], &servers[j + 1], sizeof(server_t));
                memcpy(&servers[j + 1], &temp, sizeof(server_t));
            }
        }
    }
}

void print_servers(int server_num)
{
    printf("=========================================================================================================================================================================\n");
    printf("%-5s %-60s %-15s %-15s %-15s %-15s %-15s %-15s\n", "ID", "URL", "Latitude", "Longitude", "Name", "Country", "Distance", "Latency (ms)");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < server_num; i++)
    {
        printf("%-5i %-60s %-15s %-15s %-15s %-15s %-15i %-15.2f\n", i,
               servers[i].url,
               servers[i].lat,
               servers[i].lon,
               servers[i].name,
               servers[i].country,
               servers[i].distance,
               servers[i].latency);
    }
    printf("=========================================================================================================================================================================\n");
}