status_t Parcel::readString16(std::unique_ptr<String16>* pArg) const
{
 const int32_t start = dataPosition();
 int32_t size;
 status_t status = readInt32(&size);
    pArg->reset();

 if (status != OK || size < 0) {
 return status;
 }

    setDataPosition(start);
    pArg->reset(new (std::nothrow) String16());

    status = readString16(pArg->get());

 if (status != OK) {
        pArg->reset();
 }

 return status;
}
