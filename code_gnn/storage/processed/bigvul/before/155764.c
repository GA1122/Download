v8::Local<v8::Value> SerializedScriptValue::deserialize(v8::Isolate* isolate, MessagePortArray* messagePorts, const WebBlobInfoArray* blobInfo)
{
    return SerializedScriptValueFactory::instance().deserialize(this, isolate, messagePorts, blobInfo);
}
