EventFilteringInfo ParseFromObject(v8::Local<v8::Object> object,
                                   v8::Isolate* isolate) {
  EventFilteringInfo info;
  v8::Local<v8::String> url(v8::String::NewFromUtf8(isolate, "url"));
  if (object->Has(url)) {
    v8::Local<v8::Value> url_value(object->Get(url));
    info.SetURL(GURL(*v8::String::Utf8Value(url_value)));
  }
  v8::Local<v8::String> instance_id(
      v8::String::NewFromUtf8(isolate, "instanceId"));
  if (object->Has(instance_id)) {
    v8::Local<v8::Value> instance_id_value(object->Get(instance_id));
    info.SetInstanceID(instance_id_value->IntegerValue());
  }
  v8::Local<v8::String> service_type(
      v8::String::NewFromUtf8(isolate, "serviceType"));
  if (object->Has(service_type)) {
    v8::Local<v8::Value> service_type_value(object->Get(service_type));
    info.SetServiceType(*v8::String::Utf8Value(service_type_value));
  }
  v8::Local<v8::String> window_types(
      v8::String::NewFromUtf8(isolate, "windowType"));
  if (object->Has(window_types)) {
    v8::Local<v8::Value> window_types_value(object->Get(window_types));
    info.SetWindowType(*v8::String::Utf8Value(window_types_value));
  }

  v8::Local<v8::String> window_exposed(
      v8::String::NewFromUtf8(isolate, "windowExposedByDefault"));
  if (object->Has(window_exposed)) {
    v8::Local<v8::Value> window_exposed_value(object->Get(window_exposed));
    info.SetWindowExposedByDefault(
        window_exposed_value.As<v8::Boolean>()->Value());
  }

  return info;
}
