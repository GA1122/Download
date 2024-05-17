void TestingAutomationProvider::GetSecurityState(
    int handle,
    bool* success,
    content::SecurityStyle* security_style,
    net::CertStatus* ssl_cert_status,
    int* insecure_content_status) {
  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* tab = tab_tracker_->GetResource(handle);
    NavigationEntry* entry = tab->GetActiveEntry();
    *success = true;
    *security_style = entry->GetSSL().security_style;
    *ssl_cert_status = entry->GetSSL().cert_status;
    *insecure_content_status = entry->GetSSL().content_status;
  } else {
    *success = false;
    *security_style = content::SECURITY_STYLE_UNKNOWN;
    *ssl_cert_status = 0;
    *insecure_content_status = 0;
  }
}
