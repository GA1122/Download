  static v8::Handle<v8::Value> StartRequestCommon(
      const v8::Arguments& args, ListValue* value_args) {
    ExtensionImpl* v8_extension = GetFromArguments<ExtensionImpl>(args);

    RenderView* renderview = bindings_utils::GetRenderViewForCurrentContext();
    if (!renderview)
      return v8::Undefined();

    std::string name = *v8::String::AsciiValue(args[0]);
    const std::set<std::string>& function_names =
        v8_extension->extension_dispatcher_->function_names();
    if (function_names.find(name) == function_names.end()) {
      NOTREACHED() << "Unexpected function " << name;
      return v8::Undefined();
    }

    if (!v8_extension->CheckPermissionForCurrentContext(name))
      return v8::Undefined();

    GURL source_url;
    WebFrame* webframe = WebFrame::frameForCurrentContext();
    if (webframe)
      source_url = webframe->document().url();

    int request_id = args[2]->Int32Value();
    bool has_callback = args[3]->BooleanValue();
    bool for_io_thread = args[4]->BooleanValue();

    v8::Persistent<v8::Context> current_context =
        v8::Persistent<v8::Context>::New(v8::Context::GetCurrent());
    DCHECK(!current_context.IsEmpty());
    GetPendingRequestMap()[request_id].reset(new PendingRequest(
        current_context, name));

    ExtensionHostMsg_Request_Params params;
    params.name = name;
    params.arguments.Swap(value_args);
    params.source_url = source_url;
    params.request_id = request_id;
    params.has_callback = has_callback;
    params.user_gesture =
        webframe ? webframe->isProcessingUserGesture() : false;
    if (for_io_thread) {
      renderview->Send(new ExtensionHostMsg_RequestForIOThread(
          renderview->routing_id(), params));
    } else {
      renderview->Send(new ExtensionHostMsg_Request(
          renderview->routing_id(), params));
    }

    return v8::Undefined();
  }
