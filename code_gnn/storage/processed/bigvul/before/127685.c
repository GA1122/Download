static void perWorldBindingsReadonlyTestInterfaceEmptyAttributeAttributeGetterForMainWorld(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> holder = info.Holder();
    TestInterfaceNode* impl = V8TestInterfaceNode::toImpl(holder);
    v8SetReturnValueForMainWorld(info, WTF::getPtr(impl->perWorldBindingsReadonlyTestInterfaceEmptyAttribute()));
}