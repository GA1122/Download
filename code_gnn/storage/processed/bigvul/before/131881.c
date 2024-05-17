static void voidMethodCompareHowArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        throwTypeError(ExceptionMessages::failedToExecute("voidMethodCompareHowArg", "TestObjectPython", ExceptionMessages::notEnoughArguments(1, info.Length())), info.GetIsolate());
        return;
    }
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Range::CompareHow, compareHowArg, static_cast<Range::CompareHow>(info[0]->Int32Value()));
    imp->voidMethodCompareHowArg(compareHowArg);
}
