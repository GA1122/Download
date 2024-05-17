void V8Float64Array::derefObject(void* object)
{
    static_cast<Float64Array*>(object)->deref();
}
