void RenderViewImpl::ScrollFocusedNodeIntoViewForWidget() {
  if (WebLocalFrame* focused_frame = GetWebView()->FocusedFrame()) {
    auto* frame_widget = focused_frame->LocalRoot()->FrameWidget();
    frame_widget->ScrollFocusedEditableElementIntoView();
  }
}
