PassRefPtr<SerializedScriptValue> SerializedScriptValue::createAndSwallowExceptions(v8::Handle<v8::Value> value, v8::Isolate* isolate)
{
    TrackExceptionState exceptionState;
    return adoptRef(new SerializedScriptValue(value, 0, 0, 0, exceptionState, isolate));
}
