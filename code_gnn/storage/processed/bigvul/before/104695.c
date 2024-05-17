  static v8::Handle<v8::Value> OpenChannelToTab(const v8::Arguments& args) {
    RenderView* renderview = bindings_utils::GetRenderViewForCurrentContext();
    if (!renderview)
      return v8::Undefined();

    if (args.Length() >= 3 && args[0]->IsInt32() && args[1]->IsString() &&
        args[2]->IsString()) {
      int tab_id = args[0]->Int32Value();
      std::string extension_id = *v8::String::Utf8Value(args[1]->ToString());
      std::string channel_name = *v8::String::Utf8Value(args[2]->ToString());
      int port_id = -1;
      renderview->Send(new ExtensionHostMsg_OpenChannelToTab(
          renderview->routing_id(), tab_id, extension_id, channel_name,
          &port_id));
      return v8::Integer::New(port_id);
    }
    return v8::Undefined();
  }
