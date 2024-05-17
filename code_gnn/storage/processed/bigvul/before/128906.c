PassRefPtr<SerializedScriptValue> SerializedScriptValue::create(v8::Handle<v8::Value> value, MessagePortArray* messagePorts, ArrayBufferArray* arrayBuffers, ExceptionState& exceptionState, v8::Isolate* isolate)
{
    return adoptRef(new SerializedScriptValue(value, messagePorts, arrayBuffers, 0, exceptionState, isolate));
}
