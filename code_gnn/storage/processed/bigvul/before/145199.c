void WebstoreBindings::Install(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  content::RenderFrame* render_frame = context()->GetRenderFrame();
  if (!render_frame)
    return;

  int listener_mask = 0;
  CHECK(args[0]->IsBoolean());
  if (args[0]->BooleanValue())
    listener_mask |= api::webstore::INSTALL_STAGE_LISTENER;
  CHECK(args[1]->IsBoolean());
  if (args[1]->BooleanValue())
    listener_mask |= api::webstore::DOWNLOAD_PROGRESS_LISTENER;

  std::string preferred_store_link_url;
  if (!args[2]->IsUndefined()) {
    CHECK(args[2]->IsString());
    preferred_store_link_url = std::string(*v8::String::Utf8Value(args[2]));
  }

  std::string webstore_item_id;
  std::string error;
  blink::WebLocalFrame* frame = context()->web_frame();

  if (!GetWebstoreItemIdFromFrame(
      frame, preferred_store_link_url, &webstore_item_id, &error)) {
    args.GetIsolate()->ThrowException(
        v8::String::NewFromUtf8(args.GetIsolate(), error.c_str()));
    return;
  }

  int install_id = g_next_install_id++;

  Send(new ExtensionHostMsg_InlineWebstoreInstall(
      render_frame->GetRoutingID(), install_id, GetRoutingID(),
      webstore_item_id, frame->document().url(), listener_mask));

  args.GetReturnValue().Set(static_cast<int32_t>(install_id));
}
