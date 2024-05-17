status_t Parcel::writeStrongBinderVector(const std::vector<sp<IBinder>>& val)
{
 return writeTypedVector(val, &Parcel::writeStrongBinder);
}
