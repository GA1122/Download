void ChromeClientImpl::UpdateEventRectsForSubframeIfNecessary(
    LocalFrame* frame) {
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  WebFrameWidgetBase* widget = web_frame->LocalRoot()->FrameWidget();
  if (WebLayerTreeView* tree_view = widget->GetLayerTreeView())
    tree_view->UpdateEventRectsForSubframeIfNecessary();
}
