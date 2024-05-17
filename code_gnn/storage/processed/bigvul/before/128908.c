PassRefPtr<SerializedScriptValue> SerializedScriptValue::create(const String& data)
{
    return create(data, v8::Isolate::GetCurrent());
}
