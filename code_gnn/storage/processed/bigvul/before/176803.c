status_t Parcel::writeUniqueFileDescriptor(const ScopedFd& fd) {
 return writeDupFileDescriptor(fd.get());
}
