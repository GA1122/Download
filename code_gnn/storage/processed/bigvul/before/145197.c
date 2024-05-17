 void TabsCustomBindings::OpenChannelToTab(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  content::RenderFrame* render_frame = context()->GetRenderFrame();
  if (!render_frame)
    return;

  CHECK(args.Length() >= 4 &&
        args[0]->IsInt32() &&
        args[1]->IsInt32() &&
        args[2]->IsString() &&
        args[3]->IsString());

  ExtensionMsg_TabTargetConnectionInfo info;
  info.tab_id = args[0]->Int32Value();
  info.frame_id = args[1]->Int32Value();
  std::string extension_id = *v8::String::Utf8Value(args[2]);
  std::string channel_name = *v8::String::Utf8Value(args[3]);
  int port_id = -1;
  render_frame->Send(new ExtensionHostMsg_OpenChannelToTab(
      render_frame->GetRoutingID(), info, extension_id, channel_name,
      &port_id));
  args.GetReturnValue().Set(static_cast<int32_t>(port_id));
}
