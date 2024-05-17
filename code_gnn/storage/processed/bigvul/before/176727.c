size_t Parcel::dataAvail() const
{
 size_t result = dataSize() - dataPosition();
 if (result > INT32_MAX) {
        abort();
 }
 return result;
}
