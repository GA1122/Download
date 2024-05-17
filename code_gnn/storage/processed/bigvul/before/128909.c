PassRefPtr<SerializedScriptValue> SerializedScriptValue::create(const String& data, v8::Isolate* isolate)
{
    Writer writer;
    writer.writeWebCoreString(data);
    String wireData = writer.takeWireString();
    return adoptRef(new SerializedScriptValue(wireData));
}
