
#include <string.h>

#include "cloud.h"

void mosquittopp_cloud::on_connect(int rc)
{
	printf("mosquittopp_cloud::on_connect \n");
	int mid = 0;
	subscribe(&mid, "connect/did/abc", 1);
	printf("mosquittopp_cloud::on_connect subscribe: %d \n", mid);
}

void mosquittopp_cloud::on_disconnect(int rc)
{
	printf("mosquittopp_cloud::on_disconnect \n");
}

void mosquittopp_cloud::on_publish(int mid)
{
	printf("mosquittopp_cloud::on_publish \n");
}

void mosquittopp_cloud::on_message(const struct mosquitto_message * message)
{
	printf("mosquittopp_cloud::on_message \n");
	
	printf("receive topic: %s \n", message->topic);
	printf("receive payloadlen: %d \n", message->payloadlen);
	printf("receive payload: \n");
	{
		char *str = strndup((const char *)message->payload, message->payloadlen);
		printf("%s \n", str);
		free(str);
	}
}

void mosquittopp_cloud::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	printf("mosquittopp_cloud::on_subscribe \n");
}

void mosquittopp_cloud::on_unsubscribe(int mid)
{
	printf("mosquittopp_cloud::on_unsubscribe \n");
}

void mosquittopp_cloud::on_log(int level, const char * str)
{
	printf("mosquittopp_cloud::on_log (%d, %s)\n", level, str);
}

