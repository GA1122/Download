status_t Parcel::readUtf8FromUtf16(std::unique_ptr<std::string>* str) const {
 const int32_t start = dataPosition();
 int32_t size;
 status_t status = readInt32(&size);
    str->reset();

 if (status != OK || size < 0) {
 return status;
 }

    setDataPosition(start);
    str->reset(new (std::nothrow) std::string());
 return readUtf8FromUtf16(str->get());
}
