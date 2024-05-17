static void javaScriptCallback(WKSerializedScriptValueRef valueRef, WKErrorRef, void* data)
{
    JSCallbackClosure* closure = reinterpret_cast<JSCallbackClosure*>(data);

    if (closure->method.size())
        QMetaObject::invokeMethod(closure->receiver, closure->method);
    else {
        QJSValue function = closure->value;

        if (function.isCallable()) {
            QJSValue var;
            if (valueRef) {
                JSGlobalContextRef context = JSGlobalContextCreate(0);

                JSValueRef exception = 0;
                JSValueRef value = WKSerializedScriptValueDeserialize(valueRef, context, &exception);
                var = buildQJSValue(function.engine(), context, value,   0);

                JSGlobalContextRelease(context);
            }

            QList<QJSValue> args;
            args.append(var);
            function.call(args);
        }
    }

    delete closure;
}
