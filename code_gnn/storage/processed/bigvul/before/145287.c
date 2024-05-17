void LazyBackgroundPageNativeHandler::DecrementKeepaliveCount(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (context() && ExtensionFrameHelper::IsContextForEventPage(context())) {
    content::RenderFrame* render_frame = context()->GetRenderFrame();
    render_frame->Send(new ExtensionHostMsg_DecrementLazyKeepaliveCount(
        render_frame->GetRoutingID()));
  }
}
