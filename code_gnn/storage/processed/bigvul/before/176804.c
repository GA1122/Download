status_t Parcel::writeUniqueFileDescriptorVector(const std::vector<ScopedFd>& val) {
 return writeTypedVector(val, &Parcel::writeUniqueFileDescriptor);
}
