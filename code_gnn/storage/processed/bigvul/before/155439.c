void ChromeContentBrowserClient::SelectClientCertificate(
    content::WebContents* web_contents,
    net::SSLCertRequestInfo* cert_request_info,
    net::ClientCertIdentityList client_certs,
    std::unique_ptr<content::ClientCertificateDelegate> delegate) {
  prerender::PrerenderContents* prerender_contents =
      prerender::PrerenderContents::FromWebContents(web_contents);
  if (prerender_contents) {
    prerender_contents->Destroy(
        prerender::FINAL_STATUS_SSL_CLIENT_CERTIFICATE_REQUESTED);
    return;
  }

  GURL requesting_url("https://" + cert_request_info->host_and_port.ToString());
  DCHECK(requesting_url.is_valid())
      << "Invalid URL string: https://"
      << cert_request_info->host_and_port.ToString();

  bool may_show_cert_selection = true;

  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
#if defined(OS_CHROMEOS)
  if (chromeos::ProfileHelper::IsSigninProfile(profile)) {
    may_show_cert_selection =
        chromeos::switches::IsSigninFrameClientCertUserSelectionEnabled();

    content::StoragePartition* storage_partition =
        content::BrowserContext::GetStoragePartition(
            profile, web_contents->GetSiteInstance());
    chromeos::login::SigninPartitionManager* signin_partition_manager =
        chromeos::login::SigninPartitionManager::Factory::GetForBrowserContext(
            profile);

    if (!signin_partition_manager->IsCurrentSigninStoragePartition(
            storage_partition)) {
      LOG(WARNING)
          << "Client cert requested in sign-in profile in wrong context.";
      delegate->ContinueWithCertificate(nullptr, nullptr);
      return;
    }
    VLOG(1) << "Client cert requested in sign-in profile.";
  }
#endif   

  std::unique_ptr<net::ClientCertIdentity> auto_selected_identity =
      AutoSelectCertificate(profile, requesting_url, client_certs);
  if (auto_selected_identity) {
    scoped_refptr<net::X509Certificate> cert =
        auto_selected_identity->certificate();
    net::ClientCertIdentity::SelfOwningAcquirePrivateKey(
        std::move(auto_selected_identity),
        base::Bind(&content::ClientCertificateDelegate::ContinueWithCertificate,
                   base::Passed(&delegate), std::move(cert)));
    LogClientAuthResult(ClientCertSelectionResult::kAutoSelect);
    return;
  }

  if (!may_show_cert_selection) {
    LOG(WARNING) << "No client cert matched by policy and user selection is "
                    "not allowed.";
    LogClientAuthResult(ClientCertSelectionResult::kNoSelectionAllowed);
    delegate->ContinueWithCertificate(nullptr, nullptr);
    return;
  }

  chrome::ShowSSLClientCertificateSelector(web_contents, cert_request_info,
                                           std::move(client_certs),
                                           std::move(delegate));
}
