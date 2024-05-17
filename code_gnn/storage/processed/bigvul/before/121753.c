int GetIPv4AddressFromIndex(int socket, uint32 index, uint32* address){
  if (!index) {
    *address = htonl(INADDR_ANY);
    return OK;
  }
  ifreq ifr;
  ifr.ifr_addr.sa_family = AF_INET;
  if (!if_indextoname(index, ifr.ifr_name))
    return ERR_FAILED;
  int rv = ioctl(socket, SIOCGIFADDR, &ifr);
  if (!rv)
    return MapSystemError(rv);
  *address = reinterpret_cast<sockaddr_in*>(&ifr.ifr_addr)->sin_addr.s_addr;
  return OK;
}
