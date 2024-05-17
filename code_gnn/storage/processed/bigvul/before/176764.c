status_t Parcel::readStrongBinderVector(std::vector<sp<IBinder>>* val) const {
 return readTypedVector(val, &Parcel::readStrongBinder);
}
