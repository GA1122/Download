WebLayerTreeView* ChromeClientImpl::GetWebLayerTreeView(LocalFrame* frame) {
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  return web_frame->LocalRoot()->FrameWidget()->GetLayerTreeView();
}
