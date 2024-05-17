static void attrWithSetterExceptionAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestObj.attrWithSetterException._set");
    TestObj* imp = V8TestObj::toNative(info.Holder());
    int v = toInt32(value);
    ExceptionCode ec = 0;
    imp->setAttrWithSetterException(v, ec);
    if (UNLIKELY(ec))
        V8Proxy::setDOMException(ec, info.GetIsolate());
    return;
}
