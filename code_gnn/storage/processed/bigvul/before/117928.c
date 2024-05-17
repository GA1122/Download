static v8::Handle<v8::Value> multiTransferListCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestSerializedScriptValueInterface.multiTransferList");
    TestSerializedScriptValueInterface* imp = V8TestSerializedScriptValueInterface::toNative(args.Holder());
    if (args.Length() <= 0) {
        imp->multiTransferList();
        return v8::Handle<v8::Value>();
    }
    MessagePortArray messagePortArrayTx;
    ArrayBufferArray arrayBufferArrayTx;
    if (args.Length() > 1) {
        if (!extractTransferables(args[1], messagePortArrayTx, arrayBufferArrayTx))
            return V8Proxy::throwTypeError("Could not extract transferables");
    }
    bool firstDidThrow = false;
    RefPtr<SerializedScriptValue> first = SerializedScriptValue::create(args[0], &messagePortArrayTx, &arrayBufferArrayTx, firstDidThrow, args.GetIsolate());
    if (firstDidThrow)
        return v8::Undefined();
    if (args.Length() <= 1) {
        imp->multiTransferList(first);
        return v8::Handle<v8::Value>();
    }
    if (args.Length() <= 2) {
        imp->multiTransferList(first, messagePortArrayTx);
        return v8::Handle<v8::Value>();
    }
    MessagePortArray messagePortArrayTxx;
    ArrayBufferArray arrayBufferArrayTxx;
    if (args.Length() > 3) {
        if (!extractTransferables(args[3], messagePortArrayTxx, arrayBufferArrayTxx))
            return V8Proxy::throwTypeError("Could not extract transferables");
    }
    bool secondDidThrow = false;
    RefPtr<SerializedScriptValue> second = SerializedScriptValue::create(args[2], &messagePortArrayTxx, &arrayBufferArrayTxx, secondDidThrow, args.GetIsolate());
    if (secondDidThrow)
        return v8::Undefined();
    if (args.Length() <= 3) {
        imp->multiTransferList(first, messagePortArrayTx, second);
        return v8::Handle<v8::Value>();
    }
    imp->multiTransferList(first, messagePortArrayTx, second, messagePortArrayTxx);
    return v8::Handle<v8::Value>();
}
