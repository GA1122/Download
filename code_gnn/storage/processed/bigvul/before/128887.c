SerializedScriptValue::SerializedScriptValue(v8::Handle<v8::Value> value, MessagePortArray* messagePorts, ArrayBufferArray* arrayBuffers, WebBlobInfoArray* blobInfo, ExceptionState& exceptionState, v8::Isolate* isolate)
    : m_externallyAllocatedMemory(0)
{
    Writer writer;
    Serializer::Status status;
    String errorMessage;
    {
        v8::TryCatch tryCatch;
        Serializer serializer(writer, messagePorts, arrayBuffers, blobInfo, m_blobDataHandles, tryCatch, ScriptState::current(isolate));
        status = serializer.serialize(value);
        if (status == Serializer::JSException) {
            exceptionState.rethrowV8Exception(tryCatch.Exception());
            return;
        }
        errorMessage = serializer.errorMessage();
    }
    switch (status) {
    case Serializer::InputError:
    case Serializer::DataCloneError:
        exceptionState.throwDOMException(DataCloneError, errorMessage);
        return;
    case Serializer::Success:
        m_data = writer.takeWireString();
        ASSERT(m_data.impl()->hasOneRef());
        if (arrayBuffers && arrayBuffers->size())
            m_arrayBufferContentsArray = transferArrayBuffers(*arrayBuffers, exceptionState, isolate);
        return;
    case Serializer::JSException:
        ASSERT_NOT_REACHED();
        break;
    }
    ASSERT_NOT_REACHED();
}
