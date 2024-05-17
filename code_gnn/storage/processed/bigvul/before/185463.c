  static void locationWithExceptionAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
  {
      TestObject* proxyImp = V8TestObject::toNative(info.Holder());
    TestNode* imp = WTF::getPtr(proxyImp->locationWithException());
//     RefPtr<TestNode> imp = WTF::getPtr(proxyImp->locationWithException());
      if (!imp)
          return;
      V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, cppValue, jsValue);
     imp->setHrefThrows(cppValue);
 }