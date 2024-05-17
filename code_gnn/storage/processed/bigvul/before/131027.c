static void reflectedTreatNullAsNullStringTreatUndefinedAsNullStringStringAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValueString(info, imp->fastGetAttribute(HTMLNames::reflectedtreatnullasnullstringtreatundefinedasnullstringstringattrAttr), info.GetIsolate());
}
