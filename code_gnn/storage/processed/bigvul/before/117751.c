static void supplementalNodeAttrSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.TestInterface.supplementalNode._set");
    TestInterface* imp = V8TestInterface::toNative(info.Holder());
    Node* v = V8Node::HasInstance(value) ? V8Node::toNative(v8::Handle<v8::Object>::Cast(value)) : 0;
    TestSupplemental::setSupplementalNode(imp, WTF::getPtr(v));
    return;
}
