void WebContentsImpl::OnDidDisplayInsecureContent() {
  RecordAction(base::UserMetricsAction("SSL.DisplayedInsecureContent"));
  displayed_insecure_content_ = true;
  SSLManager::NotifySSLInternalStateChanged(
      GetController().GetBrowserContext());
}
