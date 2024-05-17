void InterstitialPageImpl::SetSize(const gfx::Size& size) {
  if (!enabled())
    return;
#if !defined(OS_MACOSX)
  if (render_view_host_->GetWidget()->GetView())
    render_view_host_->GetWidget()->GetView()->SetSize(size);
#else
  NOTIMPLEMENTED();
#endif
}
