static void perWorldBindingsTestInterfaceEmptyMethodOptionalBooleanArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "perWorldBindingsTestInterfaceEmptyMethodOptionalBooleanArg", "TestInterfaceNode", info.Holder(), info.GetIsolate());
    TestInterfaceNode* impl = V8TestInterfaceNode::toImpl(info.Holder());
    bool optionalBooleanArgument;
    {
        int numArgsPassed = info.Length();
        while (numArgsPassed > 0) {
            if (!info[numArgsPassed - 1]->IsUndefined())
                break;
            --numArgsPassed;
        }
        if (UNLIKELY(numArgsPassed <= 0)) {
            v8SetReturnValueFast(info, impl->perWorldBindingsTestInterfaceEmptyMethodOptionalBooleanArg(), impl);
            return;
        }
        optionalBooleanArgument = toBoolean(info.GetIsolate(), info[0], exceptionState);
        if (exceptionState.throwIfNeeded())
            return;
    }
    v8SetReturnValueFast(info, impl->perWorldBindingsTestInterfaceEmptyMethodOptionalBooleanArg(optionalBooleanArgument), impl);
}
