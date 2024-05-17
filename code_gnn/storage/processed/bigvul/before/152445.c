void RenderFrameImpl::OnReloadLoFiImages() {
  previews_state_ = PREVIEWS_NO_TRANSFORM;
  GetWebFrame()->ReloadLoFiImages();
}
