void FrameFetchContext::DispatchDidReceiveResponse(
    unsigned long identifier,
    const ResourceResponse& response,
    network::mojom::RequestContextFrameType frame_type,
    WebURLRequest::RequestContext request_context,
    Resource* resource,
    ResourceResponseType response_type) {
  if (IsDetached())
    return;

  DCHECK(resource);

  MaybeRecordCTPolicyComplianceUseCounter(GetFrame(), resource->GetType(),
                                          response.GetCTPolicyCompliance(),
                                          MasterDocumentLoader());

  if (response_type == ResourceResponseType::kFromMemoryCache) {
    probe::markResourceAsCached(GetFrame(), MasterDocumentLoader(), identifier);
    if (response.IsNull())
      return;
  }

  MixedContentChecker::CheckMixedPrivatePublic(GetFrame(),
                                               response.RemoteIPAddress());
  LinkLoader::CanLoadResources resource_loading_policy =
      response_type == ResourceResponseType::kFromMemoryCache
          ? LinkLoader::kDoNotLoadResources
          : LinkLoader::kLoadResourcesAndPreconnect;
  if (document_loader_ &&
      document_loader_ == document_loader_->GetFrame()
                              ->Loader()
                              .GetProvisionalDocumentLoader()) {
    resource_loading_policy = LinkLoader::kDoNotLoadResources;
  }
  KURL frame_url = Url();
  if (frame_url == NullURL())
    frame_url = document_loader_->Url();

  if ((resource->GetType() == Resource::kMainResource) &&
      (IsMainFrame() || IsFirstPartyOrigin(response.Url()))) {
    ParseAndPersistClientHints(response);
  }

  LinkLoader::LoadLinksFromHeader(
      response.HttpHeaderField(HTTPNames::Link), response.Url(), *GetFrame(),
      document_, NetworkHintsInterfaceImpl(), resource_loading_policy,
      LinkLoader::kLoadAll, nullptr);

  if (response.HasMajorCertificateErrors()) {
    MixedContentChecker::HandleCertificateError(GetFrame(), response,
                                                frame_type, request_context);
  }

  if (response.IsLegacySymantecCert()) {
    RecordLegacySymantecCertUseCounter(GetFrame(), resource->GetType());
    GetLocalFrameClient()->ReportLegacySymantecCert(response.Url(),
                                                    false  );
  }

  GetFrame()->Loader().Progress().IncrementProgress(identifier, response);
  GetLocalFrameClient()->DispatchDidReceiveResponse(response);
  DocumentLoader* document_loader = MasterDocumentLoader();
  probe::didReceiveResourceResponse(GetFrame()->GetDocument(), identifier,
                                    document_loader, response, resource);
  GetFrame()->Console().ReportResourceResponseReceived(document_loader,
                                                       identifier, response);
}
