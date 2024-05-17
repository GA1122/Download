static void namedPropertyGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    if (info.Holder()->HasRealNamedProperty(name))
        return;
    if (!info.Holder()->GetRealNamedPropertyInPrototypeChain(name).IsEmpty())
        return;

    TestObject* imp = V8TestObject::toNative(info.Holder());
    AtomicString propertyName = toCoreAtomicString(name);
    String result = imp->anonymousNamedGetter(propertyName);
    if (result.isNull())
        return;
    v8SetReturnValueString(info, result, info.GetIsolate());
}
