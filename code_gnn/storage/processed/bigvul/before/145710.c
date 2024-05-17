gin::ObjectTemplateBuilder TestServiceProvider::GetObjectTemplateBuilder(
    v8::Isolate* isolate) {
  return Wrappable<TestServiceProvider>::GetObjectTemplateBuilder(isolate)
      .SetMethod("connectToService", &TestServiceProvider::ConnectToService);
}
