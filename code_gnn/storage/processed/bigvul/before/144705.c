void WebContentsImpl::UserGestureDone() {
  OnUserGesture(GetRenderViewHost()->GetWidget());
}
