status_t Parcel::readString16(String16* pArg) const
{
 size_t len;
 const char16_t* str = readString16Inplace(&len);
 if (str) {
        pArg->setTo(str, len);
 return 0;
 } else {
 *pArg = String16();
 return UNEXPECTED_NULL;
 }
}
