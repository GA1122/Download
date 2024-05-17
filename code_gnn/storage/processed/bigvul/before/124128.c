void ChromeContentBrowserClient::SelectClientCertificate(
    int render_process_id,
    int render_view_id,
    const net::HttpNetworkSession* network_session,
    net::SSLCertRequestInfo* cert_request_info,
    const base::Callback<void(net::X509Certificate*)>& callback) {
  WebContents* tab = tab_util::GetWebContentsByID(
      render_process_id, render_view_id);
  if (!tab) {
    NOTREACHED();
    return;
  }

  GURL requesting_url("https://" + cert_request_info->host_and_port);
  DCHECK(requesting_url.is_valid()) << "Invalid URL string: https://"
                                    << cert_request_info->host_and_port;

  Profile* profile = Profile::FromBrowserContext(tab->GetBrowserContext());
  scoped_ptr<Value> filter(
      profile->GetHostContentSettingsMap()->GetWebsiteSetting(
          requesting_url,
          requesting_url,
          CONTENT_SETTINGS_TYPE_AUTO_SELECT_CERTIFICATE,
          std::string(), NULL));

  if (filter.get()) {
    if (filter->IsType(Value::TYPE_DICTIONARY)) {
      DictionaryValue* filter_dict =
          static_cast<DictionaryValue*>(filter.get());

      const std::vector<scoped_refptr<net::X509Certificate> >&
          all_client_certs = cert_request_info->client_certs;
      for (size_t i = 0; i < all_client_certs.size(); ++i) {
        if (CertMatchesFilter(*all_client_certs[i], *filter_dict)) {
          callback.Run(all_client_certs[i]);
          return;
        }
      }
    } else {
      NOTREACHED();
    }
  }

  SSLTabHelper* ssl_tab_helper = SSLTabHelper::FromWebContents(tab);
  if (!ssl_tab_helper) {
    callback.Run(NULL);
    return;
  }
  ssl_tab_helper->ShowClientCertificateRequestDialog(
      network_session, cert_request_info, callback);
}
