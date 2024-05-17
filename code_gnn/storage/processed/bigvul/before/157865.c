void WebContentsImpl::SetTopControlsShownRatio(float ratio) {
  if (delegate_)
    delegate_->SetTopControlsShownRatio(this, ratio);
}
