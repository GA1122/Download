void LocalFrameClientImpl::ReportLegacySymantecCert(const KURL& url,
                                                    bool did_fail) {
  if (web_frame_->Client())
    web_frame_->Client()->ReportLegacySymantecCert(url, did_fail);
}
