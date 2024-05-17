ResourceRequest FrameLoader::ResourceRequestForReload(
    WebFrameLoadType frame_load_type,
    ClientRedirectPolicy client_redirect_policy) {
  DCHECK(IsReloadLoadType(frame_load_type));
  const auto cache_mode =
      frame_load_type == WebFrameLoadType::kReloadBypassingCache
          ? mojom::FetchCacheMode::kBypassCache
          : mojom::FetchCacheMode::kValidateCache;
  if (!document_loader_ || !document_loader_->GetHistoryItem())
    return ResourceRequest();
  ResourceRequest request =
      document_loader_->GetHistoryItem()->GenerateResourceRequest(cache_mode);

  request.SetRequestorOrigin(SecurityOrigin::Create(request.Url()));

  if (client_redirect_policy == ClientRedirectPolicy::kClientRedirect) {
    request.SetHTTPReferrer(SecurityPolicy::GenerateReferrer(
        frame_->GetDocument()->GetReferrerPolicy(),
        frame_->GetDocument()->Url(),
        frame_->GetDocument()->OutgoingReferrer()));
  }

  request.SetSkipServiceWorker(frame_load_type ==
                               WebFrameLoadType::kReloadBypassingCache);
  return request;
}
