status_t Parcel::readParcelable(Parcelable* parcelable) const {
 int32_t have_parcelable = 0;
 status_t status = readInt32(&have_parcelable);
 if (status != OK) {
 return status;
 }
 if (!have_parcelable) {
 return UNEXPECTED_NULL;
 }
 return parcelable->readFromParcel(this);
}
