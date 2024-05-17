IPAttributesGetterMac::~IPAttributesGetterMac() {
  if (IsInitialized()) {
    PCHECK(IGNORE_EINTR(close(ioctl_socket_)) == 0);
  }
}
