status_t Parcel::readDoubleVector(std::unique_ptr<std::vector<double>>* val) const {
 return readNullableTypedVector(val, &Parcel::readDouble);
}
