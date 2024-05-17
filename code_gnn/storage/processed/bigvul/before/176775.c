status_t Parcel::writeBoolVector(const std::vector<bool>& val)
{
 return writeTypedVector(val, &Parcel::writeBool);
}
