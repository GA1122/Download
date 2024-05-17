  static v8::Handle<v8::Value> GetRenderViewId(const v8::Arguments& args) {
    RenderView* renderview = bindings_utils::GetRenderViewForCurrentContext();
    if (!renderview)
      return v8::Undefined();
    return v8::Integer::New(renderview->routing_id());
  }
