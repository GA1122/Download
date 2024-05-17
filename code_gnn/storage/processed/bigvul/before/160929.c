void ChromeClientImpl::AttachRootLayer(WebLayer* root_layer,
                                       LocalFrame* local_frame) {
  WebLocalFrameImpl* web_frame =
      WebLocalFrameImpl::FromFrame(local_frame)->LocalRoot();

  DCHECK(web_frame->FrameWidget() || !root_layer);
  if (web_frame->FrameWidget())
    web_frame->FrameWidget()->SetRootLayer(root_layer);
}
