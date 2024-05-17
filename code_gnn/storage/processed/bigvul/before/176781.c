status_t writeByteVectorInternal(Parcel* parcel, const std::vector<T>& val)
{
 status_t status;
 if (val.size() > std::numeric_limits<int32_t>::max()) {
        status = BAD_VALUE;
 return status;
 }

    status = parcel->writeInt32(val.size());
 if (status != OK) {
 return status;
 }

 void* data = parcel->writeInplace(val.size());
 if (!data) {
        status = BAD_VALUE;
 return status;
 }

    memcpy(data, val.data(), val.size());
 return status;
}
