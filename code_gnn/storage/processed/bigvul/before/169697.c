v8::Local<v8::Value> V8ValueConverterImpl::ToArrayBuffer(
    v8::Isolate* isolate,
    v8::Local<v8::Object> creation_context,
    const base::BinaryValue* value) const {
  blink::WebArrayBuffer buffer =
      blink::WebArrayBuffer::create(value->GetSize(), 1);
  memcpy(buffer.data(), value->GetBuffer(), value->GetSize());
  return blink::WebArrayBufferConverter::toV8Value(
      &buffer, creation_context, isolate);
}
