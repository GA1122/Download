void ResourceDispatcherHostImpl::DidFinishLoading(ResourceLoader* loader) {
  ResourceRequestInfoImpl* info = loader->GetRequestInfo();

  if (info->GetResourceType() == RESOURCE_TYPE_MAIN_FRAME) {
    UMA_HISTOGRAM_SPARSE_SLOWLY(
        "Net.ErrorCodesForMainFrame3",
        -loader->request()->status().error());

    base::TimeDelta request_loading_time(
        base::TimeTicks::Now() - loader->request()->creation_time());
    switch (loader->request()->status().error()) {
      case net::OK:
        UMA_HISTOGRAM_LONG_TIMES(
            "Net.RequestTime2.Success", request_loading_time);
        break;
      case net::ERR_ABORTED:
        UMA_HISTOGRAM_CUSTOM_COUNTS("Net.ErrAborted.SentBytes",
                                    loader->request()->GetTotalSentBytes(), 1,
                                    50000000, 50);
        UMA_HISTOGRAM_CUSTOM_COUNTS("Net.ErrAborted.ReceivedBytes",
                                    loader->request()->GetTotalReceivedBytes(),
                                    1, 50000000, 50);
        UMA_HISTOGRAM_LONG_TIMES(
            "Net.RequestTime2.ErrAborted", request_loading_time);

        if (loader->request()->url().SchemeIsHTTPOrHTTPS()) {
          UMA_HISTOGRAM_LONG_TIMES("Net.RequestTime2.ErrAborted.HttpScheme",
                                   request_loading_time);
        } else {
          UMA_HISTOGRAM_LONG_TIMES("Net.RequestTime2.ErrAborted.NonHttpScheme",
                                   request_loading_time);
        }

        if (loader->request()->GetTotalReceivedBytes() > 0) {
          UMA_HISTOGRAM_LONG_TIMES("Net.RequestTime2.ErrAborted.NetworkContent",
                                   request_loading_time);
        } else if (loader->request()->received_response_content_length() > 0) {
          UMA_HISTOGRAM_LONG_TIMES(
              "Net.RequestTime2.ErrAborted.NoNetworkContent.CachedContent",
              request_loading_time);
        } else {
          UMA_HISTOGRAM_LONG_TIMES(
              "Net.RequestTime2.ErrAborted.NoBytesRead",
              request_loading_time);
        }

        BrowserThread::PostTask(
            BrowserThread::UI, FROM_HERE,
            base::Bind(&RecordAbortRapporOnUI, loader->request()->url(),
                       request_loading_time));
        break;
      case net::ERR_CONNECTION_RESET:
        UMA_HISTOGRAM_LONG_TIMES(
            "Net.RequestTime2.ErrConnectionReset", request_loading_time);
        break;
      case net::ERR_CONNECTION_TIMED_OUT:
        UMA_HISTOGRAM_LONG_TIMES(
            "Net.RequestTime2.ErrConnectionTimedOut", request_loading_time);
        break;
      case net::ERR_INTERNET_DISCONNECTED:
        UMA_HISTOGRAM_LONG_TIMES(
            "Net.RequestTime2.ErrInternetDisconnected", request_loading_time);
        break;
      case net::ERR_NAME_NOT_RESOLVED:
        UMA_HISTOGRAM_LONG_TIMES(
            "Net.RequestTime2.ErrNameNotResolved", request_loading_time);
        break;
      case net::ERR_TIMED_OUT:
        UMA_HISTOGRAM_LONG_TIMES(
            "Net.RequestTime2.ErrTimedOut", request_loading_time);
        break;
      default:
        UMA_HISTOGRAM_LONG_TIMES(
            "Net.RequestTime2.MiscError", request_loading_time);
        break;
    }

    if (loader->request()->url().SchemeIsCryptographic()) {
      if (loader->request()->url().host() == "www.google.com") {
        UMA_HISTOGRAM_SPARSE_SLOWLY("Net.ErrorCodesForHTTPSGoogleMainFrame2",
                                    -loader->request()->status().error());
      }

      int num_valid_scts = std::count_if(
          loader->request()->ssl_info().signed_certificate_timestamps.begin(),
          loader->request()->ssl_info().signed_certificate_timestamps.end(),
          IsValidatedSCT);
      UMA_HISTOGRAM_COUNTS_100(
          "Net.CertificateTransparency.MainFrameValidSCTCount", num_valid_scts);
    }
  } else {
    if (info->GetResourceType() == RESOURCE_TYPE_IMAGE) {
      UMA_HISTOGRAM_SPARSE_SLOWLY(
          "Net.ErrorCodesForImages",
          -loader->request()->status().error());
    }
    UMA_HISTOGRAM_SPARSE_SLOWLY(
        "Net.ErrorCodesForSubresources2",
        -loader->request()->status().error());
  }

  if (loader->request()->url().SchemeIsCryptographic()) {
    RecordCertificateHistograms(loader->request()->ssl_info(),
                                info->GetResourceType());
  }

  if (delegate_)
    delegate_->RequestComplete(loader->request());

  RemovePendingRequest(info->GetChildID(), info->GetRequestID());
}
