void V8Window::setTimeoutMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    ExceptionState es(args.GetIsolate());
    WindowSetTimeoutImpl(args, true, es);
    es.throwIfNeeded();
}
