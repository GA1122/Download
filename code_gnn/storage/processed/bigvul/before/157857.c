void WebContentsImpl::SetDisplayCutoutSafeArea(gfx::Insets insets) {
  if (display_cutout_host_impl_)
    display_cutout_host_impl_->SetDisplayCutoutSafeArea(insets);
}
