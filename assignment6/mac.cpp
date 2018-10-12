#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>

//netdevice provides low level access
using namespace std;

string intToStringHex(int x,int fill = 2){
	stringstream s;
	s << setfill('0') << setw(fill) << hex << x;
	string temp = s.str();
	temp = temp.substr(temp.length()-fill,fill);
	transform(temp.begin(), temp.end(),temp.begin(),::toupper);
	return temp;
}

string getMacAddress(char* iface)
{
   	int fd;
	//*********
	// structure of ifreq
	// http://man7.org/linux/man-pages/man7/netdevice.7.html
	//*********
	ifreq ifr; // ifr has interface name,interface address,hardware address
	
	char *mac;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy((char *)ifr.ifr_name , (const char *)iface , IFNAMSIZ-1);

	//******
	// For request explination of ioctl(here SIOCGIFHWADDR)
	// https://linux.die.net/man/7/netdevice
	//******
	ioctl(fd, SIOCGIFHWADDR, &ifr);

	close(fd);
	
	// The family at the moment contains ARPHRD_*
	mac = (char *)ifr.ifr_hwaddr.sa_data;
	
	string ret_value = "";

	//Convert to readable address
	for(int i=0;i<6;i++){
		ret_value += intToStringHex((int)mac[i]) + ":";
	}

	ret_value.pop_back();

	return ret_value;
}

int main(){
	//http://man7.org/linux/man-pages/man3/getifaddrs.3.html
	//ifaddrs contains name,address, netmask address,pointer to next interface
	ifaddrs *addrs,*tmp;

	getifaddrs(&addrs);
	tmp = addrs;

	while (tmp)
	{
	    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
	        cout<<tmp->ifa_name<<" => "<<getMacAddress(tmp->ifa_name) << endl;

	    tmp = tmp->ifa_next;
	}

	freeifaddrs(addrs);
}