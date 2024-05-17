static v8::RetainedObjectInfo* retainedDOMInfo(uint16_t classId, v8::Handle<v8::Value> wrapper)
{
    ASSERT(classId == v8DOMNodeClassId);
    if (!wrapper->IsObject())
        return 0;
    Node* node = V8Node::toNative(wrapper.As<v8::Object>());
    return node ? new RetainedDOMInfo(node) : 0;
}
