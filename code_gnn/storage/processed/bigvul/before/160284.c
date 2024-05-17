  ScriptableObject(v8::Isolate* isolate,
                   base::WeakPtr<MimeHandlerViewContainer> container)
    : gin::NamedPropertyInterceptor(isolate, this),
      container_(container) {}
