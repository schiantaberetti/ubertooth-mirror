#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "ubertooth.h"
#include <bluetooth_packet.h>
#include <bluetooth_piconet.h>
#include <getopt.h>
#include <string.h>



int main(int argc, char *argv[])
{
	struct libusb_device_handle *devh = NULL;
	char *end, ubertooth_device = -1;
	devh = ubertooth_start(atoi(argv[1]));
	if (devh == NULL) {
		return 1;
	}
	//rx_follow(devh, &pn, clock, delay);
	u8 buf[80];
	int r;
	u64 target_addr = 0x001583400048; // DCI-LAP address for GIAC
	r = cmd_set_bdaddr(devh,target_addr);
	if(r)
		printf("Address not transferred!\n");
	
	r = libusb_control_transfer(devh, CTRL_IN, 100, 0, 0,(unsigned char *)buf, 9, 1000); //activating inquiry scan

	if (r < 0) 
		if (r == LIBUSB_ERROR_PIPE) {
			fprintf(stderr, "control message unsupported\n");
		} else {
			show_libusb_error(r);
		}
	
	printf("Answer of ubertooth to inquiry scan usb command:\n%s\n",buf);

	usb_pkt_rx usb_pkt;
	while(1)
	{
		sleep(1);
		r = cmd_poll(devh,&usb_pkt);
		//r = cmd_ping(devh);
		if(r>=0 && strncmp(usb_pkt.data,"N!",2)!=0)
		{
			printf("Received: %s\n",usb_pkt.data);
			printf("Current channel: %d\n",usb_pkt.channel);
		}
	}
/*
typedef struct {
	u8     pkt_type;
	u8     status;
	u8     channel;
	u8     clkn_high;
	u32    clk100ns;
	char   rssi_max;   // Max RSSI seen while collecting symbols in this packet
	char   rssi_min;   // Min ...
	char   rssi_avg;   // Average ...
	u8     rssi_count; // Number of ... (0 means RSSI stats are invalid)
	u8     reserved[2];
	u8     data[DMA_SIZE];
} usb_pkt_rx;
*/





	ubertooth_stop(devh);

	return 0;
}
