status_t Parcel::readStrongBinderVector(std::unique_ptr<std::vector<sp<IBinder>>>* val) const {
 return readNullableTypedVector(val, &Parcel::readStrongBinder);
}
