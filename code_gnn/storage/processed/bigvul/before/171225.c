bool unmarshallFilter(const Parcel& p,
 Metadata::Filter *filter,
 status_t *status)
{
 int32_t val;
 if (p.readInt32(&val) != OK)
 {
        ALOGE("Failed to read filter's length");
 *status = NOT_ENOUGH_DATA;
 return false;
 }

 if( val > kMaxFilterSize || val < 0)
 {
        ALOGE("Invalid filter len %d", val);
 *status = BAD_VALUE;
 return false;
 }

 const size_t num = val;

    filter->clear();
    filter->setCapacity(num);

 size_t size = num * sizeof(Metadata::Type);


 if (p.dataAvail() < size)
 {
        ALOGE("Filter too short expected %zu but got %zu", size, p.dataAvail());
 *status = NOT_ENOUGH_DATA;
 return false;
 }

 const Metadata::Type *data =
 static_cast<const Metadata::Type*>(p.readInplace(size));

 if (NULL == data)
 {
        ALOGE("Filter had no data");
 *status = BAD_VALUE;
 return false;
 }

 for (size_t i = 0; i < num; ++i)
 {
        filter->add(*data);
 ++data;
 }
 *status = OK;
 return true;
}
