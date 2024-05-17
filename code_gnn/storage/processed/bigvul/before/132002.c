static void windowAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(DOMWindow*, cppValue, toDOMWindow(jsValue, info.GetIsolate()));
    imp->setWindowAttribute(WTF::getPtr(cppValue));
}
