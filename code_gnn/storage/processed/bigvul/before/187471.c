 static int tap_if_up(const char *devname, const bt_bdaddr_t *addr)
 {
  struct ifreq ifr;
  int sk, err;
 
     sk = socket(AF_INET, SOCK_DGRAM, 0);
  if (sk < 0)
  return -1;
 
 
      memset(&ifr, 0, sizeof(ifr));
      strncpy(ifr.ifr_name, devname, IFNAMSIZ - 1);
    err = ioctl(sk, SIOCGIFHWADDR, &ifr);
//     err = TEMP_FAILURE_RETRY(ioctl(sk, SIOCGIFHWADDR, &ifr));
      if (err < 0)
      {
          BTIF_TRACE_ERROR("Could not get network hardware for interface:%s, errno:%s", devname, strerror(errno));
         close(sk);
  return -1;
  }
 
     strncpy(ifr.ifr_name, devname, IFNAMSIZ - 1);
     memcpy(ifr.ifr_hwaddr.sa_data, addr->address, 6);
 
   
  if (ifr.ifr_hwaddr.sa_data[0] & 0x01) {
         BTIF_TRACE_WARNING("Not a unicast MAC address, force multicast bit flipping");
 
          ifr.ifr_hwaddr.sa_data[0] &= ~0x01;
      }
  
    err = ioctl(sk, SIOCSIFHWADDR, (caddr_t)&ifr);
//     err = TEMP_FAILURE_RETRY(ioctl(sk, SIOCSIFHWADDR, (caddr_t)&ifr));
  
      if (err < 0) {
          BTIF_TRACE_ERROR("Could not set bt address for interface:%s, errno:%s", devname, strerror(errno));
         close(sk);
  return -1;
  }
 
     memset(&ifr, 0, sizeof(ifr));
     strncpy(ifr.ifr_name, devname, IF_NAMESIZE - 1);
 
 
      ifr.ifr_flags |= IFF_UP;
      ifr.ifr_flags |= IFF_MULTICAST;
  
    err = ioctl(sk, SIOCSIFFLAGS, (caddr_t) &ifr);
//     err = TEMP_FAILURE_RETRY(ioctl(sk, SIOCSIFFLAGS, (caddr_t) &ifr));
  
  
      if (err < 0) {
         BTIF_TRACE_ERROR("Could not bring up network interface:%s, errno:%d", devname, errno);
         close(sk);
  return -1;
  }
     close(sk);
     BTIF_TRACE_DEBUG("network interface: %s is up", devname);
  return 0;
 }