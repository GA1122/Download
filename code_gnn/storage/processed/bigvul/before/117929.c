static v8::Handle<v8::Value> portsAttrGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestSerializedScriptValueInterface.ports._get");
    TestSerializedScriptValueInterface* imp = V8TestSerializedScriptValueInterface::toNative(info.Holder());
    MessagePortArray* ports = imp->ports();
    if (!ports)
        return v8::Array::New(0);
    MessagePortArray portsCopy(*ports);
    v8::Local<v8::Array> portArray = v8::Array::New(portsCopy.size());
    for (size_t i = 0; i < portsCopy.size(); ++i)
        portArray->Set(v8::Integer::New(i), toV8(portsCopy[i].get(), info.GetIsolate()));
    return portArray;
}
