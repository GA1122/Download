status_t Parcel::writeStrongBinderVector(const std::unique_ptr<std::vector<sp<IBinder>>>& val)
{
 return writeNullableTypedVector(val, &Parcel::writeStrongBinder);
}
