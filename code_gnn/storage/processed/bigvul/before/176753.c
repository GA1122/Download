status_t Parcel::readInt64Vector(std::unique_ptr<std::vector<int64_t>>* val) const {
 return readNullableTypedVector(val, &Parcel::readInt64);
}
