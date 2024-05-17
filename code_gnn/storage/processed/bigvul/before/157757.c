gfx::Size WebContentsImpl::EnterPictureInPicture(
    const viz::SurfaceId& surface_id,
    const gfx::Size& natural_size) {
  return delegate_ ? delegate_->EnterPictureInPicture(surface_id, natural_size)
                   : gfx::Size();
}
