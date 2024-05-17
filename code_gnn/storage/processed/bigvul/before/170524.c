status_t Parcel::writeString8(const String8& str)
{
 status_t err = writeInt32(str.bytes());
 if (str.bytes() > 0 && err == NO_ERROR) {
        err = write(str.string(), str.bytes()+1);
 }
 return err;
}
