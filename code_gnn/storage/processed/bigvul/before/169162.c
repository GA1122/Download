void RenderFrameHostImpl::OnExitPictureInPicture() {
  if (delegate_)
    delegate_->ExitPictureInPicture();
}
