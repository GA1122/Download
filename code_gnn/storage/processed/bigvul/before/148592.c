void WebContentsImpl::UserGestureDone() {
  OnUserInteraction(GetRenderViewHost()->GetWidget(),
                    blink::WebInputEvent::kUndefined);
}
