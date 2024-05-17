void V8Window::setIntervalMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    ExceptionState es(args.GetIsolate());
    WindowSetTimeoutImpl(args, false, es);
    es.throwIfNeeded();
}
