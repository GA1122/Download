void V8TestObject::derefObject(void* object)
{
    fromInternalPointer(object)->deref();
}
