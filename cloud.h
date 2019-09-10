
#include <stdio.h>
#include <mosquittopp.h>

class mosquittopp_cloud : public mosqpp::mosquittopp
{
	public:
		
		void on_connect(int rc);
		void on_disconnect(int rc);
		void on_publish(int mid);
		void on_message(const struct mosquitto_message * message);
		void on_subscribe(int mid, int qos_count, const int *granted_qos);
		void on_unsubscribe(int mid);
		void on_log(int level, const char * str);
};

