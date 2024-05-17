void PageCaptureCustomBindings::SendResponseAck(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK(args.Length() == 1);
  CHECK(args[0]->IsInt32());

  content::RenderFrame* render_frame = context()->GetRenderFrame();
  if (render_frame) {
    render_frame->Send(new ExtensionHostMsg_ResponseAck(
        render_frame->GetRoutingID(), args[0]->Int32Value()));
  }
}
