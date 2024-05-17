status_t flatten_binder(const sp<ProcessState>&  ,
 const sp<IBinder>& binder, Parcel* out)
{
    flat_binder_object obj;

    obj.flags = 0x7f | FLAT_BINDER_FLAG_ACCEPTS_FDS;
 if (binder != NULL) {
 IBinder *local = binder->localBinder();
 if (!local) {
 BpBinder *proxy = binder->remoteBinder();
 if (proxy == NULL) {
                ALOGE("null proxy");
 }
 const int32_t handle = proxy ? proxy->handle() : 0;
            obj.type = BINDER_TYPE_HANDLE;
            obj.binder = 0;  
            obj.handle = handle;
            obj.cookie = 0;
 } else {
            obj.type = BINDER_TYPE_BINDER;
            obj.binder = reinterpret_cast<uintptr_t>(local->getWeakRefs());
            obj.cookie = reinterpret_cast<uintptr_t>(local);
 }
 } else {
        obj.type = BINDER_TYPE_BINDER;
        obj.binder = 0;
        obj.cookie = 0;
 }

 return finish_flatten_binder(binder, obj, out);
}
