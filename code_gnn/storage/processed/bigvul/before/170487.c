wp<IBinder> Parcel::readWeakBinder() const
{
    wp<IBinder> val;
    unflatten_binder(ProcessState::self(), *this, &val);
 return val;
}
