void RuntimeCustomBindings::OpenChannelToNativeApp(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  Feature::Availability availability =
      FeatureProvider::GetPermissionFeatures()
          ->GetFeature("nativeMessaging")
          ->IsAvailableToContext(context()->extension(),
                                 context()->context_type(), context()->url());
  if (!availability.is_available())
    return;

  content::RenderFrame* render_frame = context()->GetRenderFrame();
  if (!render_frame)
    return;

  CHECK(args.Length() >= 2 && args[0]->IsString() && args[1]->IsString());

  std::string extension_id = *v8::String::Utf8Value(args[0]);
  std::string native_app_name = *v8::String::Utf8Value(args[1]);

  int port_id = -1;
  render_frame->Send(new ExtensionHostMsg_OpenChannelToNativeApp(
      render_frame->GetRoutingID(), extension_id, native_app_name, &port_id));
  args.GetReturnValue().Set(static_cast<int32_t>(port_id));
}
