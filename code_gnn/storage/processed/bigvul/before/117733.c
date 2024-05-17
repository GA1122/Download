static v8::Handle<v8::Value> attr2AttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestEventConstructor.attr2._get");
    TestEventConstructor* imp = V8TestEventConstructor::toNative(info.Holder());
    return v8String(imp->attr2(), info.GetIsolate());
}
