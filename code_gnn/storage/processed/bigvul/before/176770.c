status_t Parcel::readUtf8VectorFromUtf16Vector(std::vector<std::string>* val) const {
 return readTypedVector(val, &Parcel::readUtf8FromUtf16);
}
