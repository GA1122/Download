  static gin::Handle<TestNatives> Create(v8::Isolate* isolate,
                                         const base::Closure& quit_closure) {
    return gin::CreateHandle(isolate, new TestNatives(quit_closure));
  }
