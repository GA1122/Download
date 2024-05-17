static void perWorldBindingsReadonlyTestInterfaceEmptyAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> holder = info.Holder();
    TestInterfaceNode* impl = V8TestInterfaceNode::toImpl(holder);
    v8SetReturnValueFast(info, WTF::getPtr(impl->perWorldBindingsReadonlyTestInterfaceEmptyAttribute()), impl);
}
