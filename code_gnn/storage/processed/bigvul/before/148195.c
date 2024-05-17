static void VoidMethodXPathNSResolverArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodXPathNSResolverArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  XPathNSResolver* x_path_ns_resolver_arg;
  x_path_ns_resolver_arg = ToXPathNSResolver(ScriptState::Current(info.GetIsolate()), info[0]);
  if (!x_path_ns_resolver_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodXPathNSResolverArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "XPathNSResolver")));
    return;
  }

  impl->voidMethodXPathNSResolverArg(x_path_ns_resolver_arg);
}
