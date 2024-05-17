status_t Parcel::writeCString(const char* str)
{
 return write(str, strlen(str)+1);
}
