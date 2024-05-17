status_t Parcel::writeNoException()
{
    binder::Status status;
 return status.writeToParcel(this);
}
