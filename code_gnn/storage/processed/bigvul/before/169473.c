IPAttributesGetterMac::IPAttributesGetterMac()
    : ioctl_socket_(socket(AF_INET6, SOCK_DGRAM, 0)) {
  DCHECK_GE(ioctl_socket_, 0);
}
