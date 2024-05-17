static void testObjAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValueFast(info, WTF::getPtr(imp->testObjAttr()), imp);
}
