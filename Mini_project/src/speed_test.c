#include "speed_test.h"
#include "speed_test_utils.h"
#include "speed_test_server.h"
#include "speed_test_download_upload.h"
#include "input.h"

void print_nearest_servers_table(server_data_t *nearest_servers)
{
    printf("===================================================================================================================================================================\n");
    printf("%-5s %-70s %-15s %-15s %-15s %-15s %-15s %-15s\n", "ID", "URL", "Latitude", "Longitude", "Name", "Country", "Distance", "Latency");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        printf("%-5i %-70s %-15.2f %-15.2f %-15s %-15s %-15.2f %-15i\n", i,
               nearest_servers[i].url,
               nearest_servers[i].latitude,
               nearest_servers[i].longitude,
               nearest_servers[i].name,
               nearest_servers[i].country,
               nearest_servers[i].distance,
               nearest_servers[i].latency);
    }
    printf("===================================================================================================================================================================\n");
}

void run(int thread_num, int protocol, bool auto_pick_server)
{
    int i, best_server_index;
    client_data_t client_data;
    server_data_t nearest_servers[NEAREST_SERVERS_NUM];
    pthread_t pid;
    struct sockaddr_in servinfo;
    struct itimerval timerVal;
    int count = 0;
    int choose = 0;

    memset(&client_data, 0, sizeof(client_data_t));
    for (i = 0; i < NEAREST_SERVERS_NUM; i++)
    {
        memset(&nearest_servers[i], 0, sizeof(server_data_t));
    }

    // Choose http or https protocol
    switch (protocol)
    {
    case 1:
        // http
        if (get_ipv4_addr(SPEEDTEST_DOMAIN_NAME, &servinfo))
        {
            if (!get_http_file(&servinfo, SPEEDTEST_DOMAIN_NAME, CONFIG_REQUEST_URL, CONFIG_REQUEST_URL))
            {
                printf("Can't get your IP address information.\n");
                return;
            }
        }

        if (get_ipv4_addr(SPEEDTEST_SERVERS_DOMAIN_NAME, &servinfo))
        {
            if (!get_http_file(&servinfo, SPEEDTEST_SERVERS_DOMAIN_NAME, SERVERS_LOCATION_REQUEST_URL, SERVERS_LOCATION_REQUEST_URL))
            {
                printf("Can't get servers list.\n");
                return;
            }
        }
        break;
    case 2:
        // https
        if (get_ipv4_addr_https(SPEEDTEST_DOMAIN_NAME, &servinfo))
        {
            if (!get_https_file(&servinfo, SPEEDTEST_DOMAIN_NAME, CONFIG_REQUEST_URL, CONFIG_REQUEST_URL))
            {
                printf("Can't get your IP address information.\n");
                return;
            }
        }

        if (get_ipv4_addr_https(SPEEDTEST_SERVERS_DOMAIN_NAME, &servinfo))
        {
            if (!get_https_file(&servinfo, SPEEDTEST_SERVERS_DOMAIN_NAME, SERVERS_LOCATION_REQUEST_URL, SERVERS_LOCATION_REQUEST_URL))
            {
                printf("Can't get servers list.\n");
                return;
            }
        }
        break;
    default:
        return;
    }

    get_ip_address_position(CONFIG_REQUEST_URL, &client_data);
    printf("============================================\n");
    printf("Your IP Address : %s\n", client_data.ipAddr);
    printf("Your IP Location: %0.4lf, %0.4lf\n", client_data.latitude, client_data.longitude);
    printf("Your ISP        : %s\n", client_data.isp);
    printf("============================================\n");

    if (get_nearest_server(client_data.latitude, client_data.longitude, nearest_servers) == 0)
    {
        printf("Can't get server list.\n");
        return;
    }

    if ((best_server_index = get_best_server(nearest_servers)) != -1)
    {        
        print_nearest_servers_table(nearest_servers);

        // Choose server manually
        if (!auto_pick_server)
        {
            printf("Choose server to test: ");
            get_input_int(&choose, 0, NEAREST_SERVERS_NUM);

            if (check_server(nearest_servers[choose]) == 1)
            {
                printf("Server not available\n");
                return;
            }

            pthread_create(&pid, NULL, calculate_dl_speed_thread, NULL);
            speedtest_download(&nearest_servers[choose], thread_num);

            sleep(2);
            printf("\n");
            stop_all_thread();

            pthread_create(&pid, NULL, calculate_ul_speed_thread, NULL);
            speedtest_upload(&nearest_servers[choose], thread_num);
            printf("\n");
        }
        else // Auto choose best server
        {
            // Check if can connect to server
            count = 0;
            while (check_server(nearest_servers[count]) == 1)
            {
                count++;
                if (count >= NEAREST_SERVERS_NUM)
                {
                    printf("No server available\n");
                    return;
                }
            }

            pthread_create(&pid, NULL, calculate_dl_speed_thread, NULL);
            speedtest_download(&nearest_servers[count], thread_num);

            sleep(2);
            printf("\n");
            stop_all_thread();

            pthread_create(&pid, NULL, calculate_ul_speed_thread, NULL);
            speedtest_upload(&nearest_servers[count], thread_num);
            printf("\n");
        }
    }
}