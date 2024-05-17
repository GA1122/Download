    StateBase* writeAndGreyArrayBufferView(v8::Handle<v8::Object> object, StateBase* next)
    {
        ASSERT(!object.IsEmpty());
        ArrayBufferView* arrayBufferView = V8ArrayBufferView::toNative(object);
        if (!arrayBufferView)
            return 0;
        if (!arrayBufferView->buffer())
            return handleError(DataCloneError, "An ArrayBuffer could not be cloned.", next);
        v8::Handle<v8::Value> underlyingBuffer = toV8(arrayBufferView->buffer(), m_scriptState->context()->Global(), isolate());
        if (underlyingBuffer.IsEmpty())
            return handleError(DataCloneError, "An ArrayBuffer could not be cloned.", next);
        StateBase* stateOut = doSerializeArrayBuffer(underlyingBuffer, next);
        if (stateOut)
            return stateOut;
        m_writer.writeArrayBufferView(*arrayBufferView);
        greyObject(object);
        return 0;
    }
