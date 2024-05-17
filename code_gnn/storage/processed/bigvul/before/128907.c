PassRefPtr<SerializedScriptValue> SerializedScriptValue::create(const ScriptValue& value, WebBlobInfoArray* blobInfo, ExceptionState& exceptionState, v8::Isolate* isolate)
{
    ASSERT(isolate->InContext());
    return adoptRef(new SerializedScriptValue(value.v8Value(), 0, 0, blobInfo, exceptionState, isolate));
}
