blink::WebPageVisibilityState RenderFrameHostImpl::GetVisibilityState() {
  RenderFrameHostImpl* frame = this;
  while (frame) {
    if (frame->render_widget_host_)
      break;
    frame = frame->GetParent();
  }
  if (!frame)
    return blink::kWebPageVisibilityStateHidden;

  blink::WebPageVisibilityState visibility_state =
      GetRenderWidgetHost()->is_hidden()
          ? blink::kWebPageVisibilityStateHidden
          : blink::kWebPageVisibilityStateVisible;
  GetContentClient()->browser()->OverridePageVisibilityState(this,
                                                             &visibility_state);
  return visibility_state;
}
