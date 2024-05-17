void WebContentsImpl::DidChangeVisibleSSLState() {
  if (delegate_) {
    delegate_->VisibleSSLStateChanged(this);

    SecurityStyleExplanations security_style_explanations;
    SecurityStyle security_style =
        delegate_->GetSecurityStyle(this, &security_style_explanations);
    FOR_EACH_OBSERVER(
        WebContentsObserver, observers_,
        SecurityStyleChanged(security_style, security_style_explanations));
  }
}
