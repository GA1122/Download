status_t Parcel::writeStrongBinder(const sp<IBinder>& val)
{
 return flatten_binder(ProcessState::self(), val, this);
}
