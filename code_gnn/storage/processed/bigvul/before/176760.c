status_t Parcel::readString16Vector(std::vector<String16>* val) const {
 return readTypedVector(val, &Parcel::readString16);
}
