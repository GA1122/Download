void acquire_object(const sp<ProcessState>& proc,
 const flat_binder_object& obj, const void* who)
{
    acquire_object(proc, obj, who, NULL);
}
