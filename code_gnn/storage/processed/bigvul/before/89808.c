void PCPPublicAddressChanged(int * sockets, int n_sockets, int socket6)
#else  
void PCPPublicAddressChanged(int * sockets, int n_sockets)
#endif
{
	 
	epoch_origin = upnp_time();
#ifdef ENABLE_IPV6
	PCPSendUnsolicitedAnnounce(sockets, n_sockets, socket6);
#else  
	PCPSendUnsolicitedAnnounce(sockets, n_sockets);
#endif
}
