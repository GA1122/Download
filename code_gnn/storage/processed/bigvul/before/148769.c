void InterstitialPageImpl::Focus() {
  if (!enabled())
    return;
  render_view_host_->GetWidget()->GetView()->Focus();
}
