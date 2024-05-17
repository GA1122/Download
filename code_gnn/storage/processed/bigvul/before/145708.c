gin::Handle<TestServiceProvider> TestServiceProvider::Create(
    v8::Isolate* isolate) {
  return gin::CreateHandle(isolate, new TestServiceProvider());
}
