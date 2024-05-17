status_t Parcel::writeUniqueFileDescriptorVector(const std::unique_ptr<std::vector<ScopedFd>>& val) {
 return writeNullableTypedVector(val, &Parcel::writeUniqueFileDescriptor);
}
