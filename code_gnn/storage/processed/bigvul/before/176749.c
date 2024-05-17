status_t Parcel::readFloatVector(std::unique_ptr<std::vector<float>>* val) const {
 return readNullableTypedVector(val, &Parcel::readFloat);
}
