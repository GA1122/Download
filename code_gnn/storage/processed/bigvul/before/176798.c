status_t Parcel::writeString16(const std::unique_ptr<String16>& str)
{
 if (!str) {
 return writeInt32(-1);
 }

 return writeString16(*str);
}
