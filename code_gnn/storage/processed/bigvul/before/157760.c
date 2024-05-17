void WebContentsImpl::ExitPictureInPicture() {
  if (delegate_)
    delegate_->ExitPictureInPicture();
}
