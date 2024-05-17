status_t unflatten_binder(const sp<ProcessState>& proc,
 const Parcel& in, wp<IBinder>* out)
{
 const flat_binder_object* flat = in.readObject(false);

 if (flat) {
 switch (flat->type) {
 case BINDER_TYPE_BINDER:
 *out = reinterpret_cast<IBinder*>(flat->cookie);
 return finish_unflatten_binder(NULL, *flat, in);
 case BINDER_TYPE_WEAK_BINDER:
 if (flat->binder != 0) {
                    out->set_object_and_refs(
 reinterpret_cast<IBinder*>(flat->cookie),
 reinterpret_cast<RefBase::weakref_type*>(flat->binder));
 } else {
 *out = NULL;
 }
 return finish_unflatten_binder(NULL, *flat, in);
 case BINDER_TYPE_HANDLE:
 case BINDER_TYPE_WEAK_HANDLE:
 *out = proc->getWeakProxyForHandle(flat->handle);
 return finish_unflatten_binder(
 static_cast<BpBinder*>(out->unsafe_get()), *flat, in);
 }
 }
 return BAD_TYPE;
}
