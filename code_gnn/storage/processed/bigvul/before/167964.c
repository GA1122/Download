void LocalFrame::Reload(WebFrameLoadType load_type,
                        ClientRedirectPolicy client_redirect_policy) {
  DCHECK(IsReloadLoadType(load_type));
  if (client_redirect_policy == ClientRedirectPolicy::kNotClientRedirect) {
    if (!loader_.GetDocumentLoader()->GetHistoryItem())
      return;
    FrameLoadRequest request = FrameLoadRequest(
        nullptr,
        loader_.ResourceRequestForReload(load_type, client_redirect_policy));
    request.SetClientRedirect(client_redirect_policy);
    loader_.StartNavigation(request, load_type);
  } else {
    DCHECK_EQ(WebFrameLoadType::kReload, load_type);
    navigation_scheduler_->ScheduleReload();
  }
}
