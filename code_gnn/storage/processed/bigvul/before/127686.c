static void perWorldBindingsTestInterfaceEmptyMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestInterfaceNode* impl = V8TestInterfaceNode::toImpl(info.Holder());
    v8SetReturnValueFast(info, impl->perWorldBindingsTestInterfaceEmptyMethod(), impl);
}
