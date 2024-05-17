void SSLManager::DidCommitProvisionalLoad(
    const content::NotificationDetails& in_details) {
  content::LoadCommittedDetails* details =
      content::Details<content::LoadCommittedDetails>(in_details).ptr();

  NavigationEntryImpl* entry =
      NavigationEntryImpl::FromNavigationEntry(controller_->GetActiveEntry());

  if (details->is_main_frame) {
    if (entry) {
      int ssl_cert_id;
      net::CertStatus ssl_cert_status;
      int ssl_security_bits;
      int ssl_connection_status;
      content::DeserializeSecurityInfo(details->serialized_security_info,
                                       &ssl_cert_id,
                                       &ssl_cert_status,
                                       &ssl_security_bits,
                                       &ssl_connection_status);

      entry->GetSSL() = SSLStatus();
      entry->GetSSL().cert_id = ssl_cert_id;
      entry->GetSSL().cert_status = ssl_cert_status;
      entry->GetSSL().security_bits = ssl_security_bits;
      entry->GetSSL().connection_status = ssl_connection_status;
    }
  }

  UpdateEntry(entry);
}
