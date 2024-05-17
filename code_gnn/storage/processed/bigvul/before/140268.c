void V8Window::toStringMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    v8::Handle<v8::Object> domWrapper = V8Window::findInstanceInPrototypeChain(info.This(), info.GetIsolate());
    if (domWrapper.IsEmpty()) {
        v8SetReturnValue(info, info.This()->ObjectProtoToString());
        return;
    }
    v8SetReturnValue(info, domWrapper->ObjectProtoToString());
}
