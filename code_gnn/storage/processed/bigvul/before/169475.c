bool IPAttributesGetterMac::IsInitialized() const {
  return ioctl_socket_ >= 0;
}
