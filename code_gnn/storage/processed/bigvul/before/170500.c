status_t unflatten_binder(const sp<ProcessState>& proc,
 const Parcel& in, sp<IBinder>* out)
{
 const flat_binder_object* flat = in.readObject(false);

 if (flat) {
 switch (flat->type) {
 case BINDER_TYPE_BINDER:
 *out = reinterpret_cast<IBinder*>(flat->cookie);
 return finish_unflatten_binder(NULL, *flat, in);
 case BINDER_TYPE_HANDLE:
 *out = proc->getStrongProxyForHandle(flat->handle);
 return finish_unflatten_binder(
 static_cast<BpBinder*>(out->get()), *flat, in);
 }
 }
 return BAD_TYPE;
}
