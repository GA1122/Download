  void CheckWhitelists() {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);

    if (!database_manager_.get()) {
      PostFinishTask(UNKNOWN, REASON_SB_DISABLED);
      return;
    }

    const GURL& url = url_chain_.back();
    if (url.is_valid() && database_manager_->MatchDownloadWhitelistUrl(url)) {
      DVLOG(2) << url << " is on the download whitelist.";
      RecordCountOfSignedOrWhitelistedDownload();
      PostFinishTask(SAFE, REASON_WHITELISTED_URL);
      return;
    }

    if (signature_info_.trusted()) {
      RecordCountOfSignedOrWhitelistedDownload();
      for (int i = 0; i < signature_info_.certificate_chain_size(); ++i) {
        if (CertificateChainIsWhitelisted(
                signature_info_.certificate_chain(i))) {
          PostFinishTask(SAFE, REASON_TRUSTED_EXECUTABLE);
          return;
        }
      }
    }

    if (!pingback_enabled_) {
      PostFinishTask(UNKNOWN, REASON_PING_DISABLED);
      return;
    }

#if defined(OS_WIN) || defined(OS_MACOSX)
    BrowserThread::PostTask(
        BrowserThread::UI,
        FROM_HERE,
        base::Bind(&CheckClientDownloadRequest::GetTabRedirects, this));
#else
    PostFinishTask(UNKNOWN, REASON_OS_NOT_SUPPORTED);
#endif
  }
