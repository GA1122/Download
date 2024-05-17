inline static status_t finish_flatten_binder(
 const sp<IBinder>&  , const flat_binder_object& flat, Parcel* out)
{
 return out->writeObject(flat, false);
}
