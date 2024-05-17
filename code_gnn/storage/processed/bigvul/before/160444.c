blink::mojom::PageVisibilityState RenderFrameHostImpl::GetVisibilityState() {
  RenderFrameHostImpl* frame = this;
  while (frame) {
    if (frame->render_widget_host_)
      break;
    frame = frame->GetParent();
  }
  if (!frame)
    return blink::mojom::PageVisibilityState::kHidden;

  blink::mojom::PageVisibilityState visibility_state =
      GetRenderWidgetHost()->is_hidden()
          ? blink::mojom::PageVisibilityState::kHidden
          : blink::mojom::PageVisibilityState::kVisible;
  GetContentClient()->browser()->OverridePageVisibilityState(this,
                                                             &visibility_state);
  return visibility_state;
}
