void FrameFetchContext::DispatchDidFail(const KURL& url,
                                        unsigned long identifier,
                                        const ResourceError& error,
                                        int64_t encoded_data_length,
                                        bool is_internal_request) {
  if (IsDetached())
    return;

  if (DocumentLoader* loader = MasterDocumentLoader()) {
    if (NetworkUtils::IsCertificateTransparencyRequiredError(
            error.ErrorCode())) {
      loader->GetUseCounter().Count(
          WebFeature::kCertificateTransparencyRequiredErrorOnResourceLoad,
          GetFrame());
    }

    if (NetworkUtils::IsLegacySymantecCertError(error.ErrorCode())) {
      loader->GetUseCounter().Count(
          WebFeature::kDistrustedLegacySymantecSubresource, GetFrame());
      GetLocalFrameClient()->ReportLegacySymantecCert(url, true  );
    }
  }

  GetFrame()->Loader().Progress().CompleteProgress(identifier);
  probe::didFailLoading(GetFrame()->GetDocument(), identifier,
                        MasterDocumentLoader(), error);
  if (document_) {
    InteractiveDetector* interactive_detector(
        InteractiveDetector::From(*document_));
    if (interactive_detector) {
      interactive_detector->OnResourceLoadEnd(base::nullopt);
    }
  }
  if (!is_internal_request) {
    GetFrame()->Console().DidFailLoading(MasterDocumentLoader(), identifier,
                                         error);
  }
}
