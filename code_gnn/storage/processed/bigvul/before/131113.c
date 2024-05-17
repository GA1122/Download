static void variadicNodeMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("variadicNodeMethod", "TestObject", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(Node*, head, V8Node::toNativeWithTypeCheck(info.GetIsolate(), info[0]));
    Vector<RefPtr<Node> > tail;
    for (int i = 1; i < info.Length(); ++i) {
        if (!V8Node::hasInstance(info[i], info.GetIsolate())) {
            throwTypeError(ExceptionMessages::failedToExecute("variadicNodeMethod", "TestObject", "parameter 2 is not of type 'Node'."), info.GetIsolate());
            return;
        }
        tail.append(V8Node::toNative(v8::Handle<v8::Object>::Cast(info[i])));
    }
    imp->variadicNodeMethod(head, tail);
}
