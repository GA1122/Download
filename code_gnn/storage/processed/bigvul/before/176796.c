status_t Parcel::writeParcelable(const Parcelable& parcelable) {
 status_t status = writeInt32(1);  
 if (status != OK) {
 return status;
 }
 return parcelable.writeToParcel(this);
}
