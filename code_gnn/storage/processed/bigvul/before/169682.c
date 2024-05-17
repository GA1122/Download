base::Value* V8ValueConverterImpl::FromV8ArrayBuffer(
    v8::Local<v8::Object> val,
    v8::Isolate* isolate) const {
  if (strategy_) {
    base::Value* out = NULL;
    if (strategy_->FromV8ArrayBuffer(val, &out, isolate))
      return out;
  }

  char* data = NULL;
  size_t length = 0;

  std::unique_ptr<blink::WebArrayBuffer> array_buffer(
      blink::WebArrayBufferConverter::createFromV8Value(val, isolate));
  std::unique_ptr<blink::WebArrayBufferView> view;
  if (array_buffer) {
    data = reinterpret_cast<char*>(array_buffer->data());
    length = array_buffer->byteLength();
  } else {
    view.reset(blink::WebArrayBufferView::createFromV8Value(val));
    if (view) {
      data = reinterpret_cast<char*>(view->baseAddress()) + view->byteOffset();
      length = view->byteLength();
    }
  }

  if (data)
    return base::BinaryValue::CreateWithCopiedBuffer(data, length);
  else
    return NULL;
}
