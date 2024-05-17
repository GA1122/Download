status_t Parcel::writeUtf8AsUtf16(const std::unique_ptr<std::string>& str) {
 if (!str) {
 return writeInt32(-1);
 }
 return writeUtf8AsUtf16(*str);
}
