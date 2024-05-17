static v8::Handle<v8::Value> TestObjConstructorGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.constructors._get");
    v8::Handle<v8::Value> data = info.Data();
    ASSERT(data->IsExternal() || data->IsNumber());
    WrapperTypeInfo* type = WrapperTypeInfo::unwrap(data);
    return v8::Handle<v8::Value>();}
