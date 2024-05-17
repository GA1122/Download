status_t flatten_binder(const sp<ProcessState>&  ,
 const wp<IBinder>& binder, Parcel* out)
{
    flat_binder_object obj;

    obj.flags = 0x7f | FLAT_BINDER_FLAG_ACCEPTS_FDS;
 if (binder != NULL) {
        sp<IBinder> real = binder.promote();
 if (real != NULL) {
 IBinder *local = real->localBinder();
 if (!local) {
 BpBinder *proxy = real->remoteBinder();
 if (proxy == NULL) {
                    ALOGE("null proxy");
 }
 const int32_t handle = proxy ? proxy->handle() : 0;
                obj.type = BINDER_TYPE_WEAK_HANDLE;
                obj.binder = 0;  
                obj.handle = handle;
                obj.cookie = 0;
 } else {
                obj.type = BINDER_TYPE_WEAK_BINDER;
                obj.binder = reinterpret_cast<uintptr_t>(binder.get_refs());
                obj.cookie = reinterpret_cast<uintptr_t>(binder.unsafe_get());
 }
 return finish_flatten_binder(real, obj, out);
 }

        ALOGE("Unable to unflatten Binder weak reference!");
        obj.type = BINDER_TYPE_BINDER;
        obj.binder = 0;
        obj.cookie = 0;
 return finish_flatten_binder(NULL, obj, out);

 } else {
        obj.type = BINDER_TYPE_BINDER;
        obj.binder = 0;
        obj.cookie = 0;
 return finish_flatten_binder(NULL, obj, out);
 }
}
