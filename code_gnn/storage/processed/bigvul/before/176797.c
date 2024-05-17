status_t Parcel::writeRawNullableParcelable(const Parcelable* parcelable) {
 if (!parcelable) {
 return writeInt32(0);
 }

 return writeParcelable(*parcelable);
}
