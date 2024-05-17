v8::Local<v8::Object> RectToV8Object(v8::Isolate* isolate,
                                     const gfx::Rect& rect) {
  v8::Local<v8::Object> result(v8::Object::New(isolate));
  result->Set(CreateV8String(isolate, "left"),
              v8::Integer::New(isolate, rect.x()));
  result->Set(CreateV8String(isolate, "top"),
              v8::Integer::New(isolate, rect.y()));
  result->Set(CreateV8String(isolate, "width"),
              v8::Integer::New(isolate, rect.width()));
  result->Set(CreateV8String(isolate, "height"),
              v8::Integer::New(isolate, rect.height()));
  return result;
}
