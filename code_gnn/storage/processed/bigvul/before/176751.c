status_t Parcel::readInt32Vector(std::unique_ptr<std::vector<int32_t>>* val) const {
 return readNullableTypedVector(val, &Parcel::readInt32);
}
