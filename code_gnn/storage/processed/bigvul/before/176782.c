status_t writeByteVectorInternalPtr(Parcel* parcel,
 const std::unique_ptr<std::vector<T>>& val)
{
 if (!val) {
 return parcel->writeInt32(-1);
 }

 return writeByteVectorInternal(parcel, *val);
}
