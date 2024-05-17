status_t Parcel::readCharVector(std::vector<char16_t>* val) const {
 return readTypedVector(val, &Parcel::readChar);
}
