void V8TestObjectPython::derefObject(void* object)
{
    fromInternalPointer(object)->deref();
}
