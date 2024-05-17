void V8Window::postMessageMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    DOMWindow* window = V8Window::toNative(args.Holder());
    DOMWindow* source = activeDOMWindow();

    if (!source) {
        throwTypeError(args.GetIsolate());
        return;
    }

    MessagePortArray portArray;
    ArrayBufferArray arrayBufferArray;
    int targetOriginArgIndex = 1;
    if (args.Length() > 2) {
        int transferablesArgIndex = 2;
        if (isLegacyTargetOriginDesignation(args[2])) {
            targetOriginArgIndex = 2;
            transferablesArgIndex = 1;
        }
        if (!extractTransferables(args[transferablesArgIndex], portArray, arrayBufferArray, args.GetIsolate()))
            return;
    }
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<WithUndefinedOrNullCheck>, targetOrigin, args[targetOriginArgIndex]);

    bool didThrow = false;
    RefPtr<SerializedScriptValue> message =
        SerializedScriptValue::create(args[0], &portArray, &arrayBufferArray, didThrow, args.GetIsolate());
    if (didThrow)
        return;

    ExceptionState es(args.GetIsolate());
    window->postMessage(message.release(), &portArray, targetOrigin, source, es);
    es.throwIfNeeded();
}
