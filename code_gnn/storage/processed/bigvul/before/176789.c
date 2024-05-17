status_t Parcel::writeFloatVector(const std::vector<float>& val)
{
 return writeTypedVector(val, &Parcel::writeFloat);
}
