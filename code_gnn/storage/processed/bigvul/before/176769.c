status_t Parcel::readUtf8VectorFromUtf16Vector(
        std::unique_ptr<std::vector<std::unique_ptr<std::string>>>* val) const {
 return readNullableTypedVector(val, &Parcel::readUtf8FromUtf16);
}
