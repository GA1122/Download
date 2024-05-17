static void perWorldBindingsTestInterfaceEmptyMethodMethodForMainWorld(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestInterfaceNode* impl = V8TestInterfaceNode::toImpl(info.Holder());
    v8SetReturnValueForMainWorld(info, impl->perWorldBindingsTestInterfaceEmptyMethod());
}
