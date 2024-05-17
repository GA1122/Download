v8::Local<v8::Object> GetOrCreateDistillerObject(v8::Isolate* isolate,
                                                 v8::Local<v8::Object> global) {
  v8::Local<v8::Object> distiller_obj;
  v8::Local<v8::Value> distiller_value =
      global->Get(gin::StringToV8(isolate, "distiller"));
  if (distiller_value.IsEmpty() || !distiller_value->IsObject()) {
    distiller_obj = v8::Object::New(isolate);
    global->Set(gin::StringToSymbol(isolate, "distiller"), distiller_obj);
  } else {
    distiller_obj = v8::Local<v8::Object>::Cast(distiller_value);
  }
  return distiller_obj;
}
