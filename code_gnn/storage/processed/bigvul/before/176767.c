status_t Parcel::readUniqueFileDescriptorVector(std::vector<ScopedFd>* val) const {
 return readTypedVector(val, &Parcel::readUniqueFileDescriptor);
}
