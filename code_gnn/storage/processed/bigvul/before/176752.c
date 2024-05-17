status_t Parcel::readInt32Vector(std::vector<int32_t>* val) const {
 return readTypedVector(val, &Parcel::readInt32);
}
