static void reflectedTreatNullAsNullStringURLAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValueString(info, imp->getURLAttribute(HTMLNames::reflectedtreatnullasnullstringurlattrAttr), info.GetIsolate());
}
