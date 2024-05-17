void FrameFetchContext::AddAdditionalRequestHeaders(ResourceRequest& request,
                                                    FetchResourceType type) {
  BaseFetchContext::AddAdditionalRequestHeaders(request, type);

  if (!request.Url().IsEmpty() && !request.Url().ProtocolIsInHTTPFamily())
    return;

  if (IsDetached())
    return;

  if (IsReloadLoadType(MasterDocumentLoader()->LoadType()))
    request.ClearHTTPHeaderField(HTTPNames::Save_Data);

  if (save_data_enabled_)
    request.SetHTTPHeaderField(HTTPNames::Save_Data, "on");

  if (GetLocalFrameClient()->GetPreviewsStateForFrame() &
      WebURLRequest::kNoScriptOn) {
    request.AddHTTPHeaderField(
        "Intervention",
        "<https://www.chromestatus.com/features/4775088607985664>; "
        "level=\"warning\"");
  }

  if (GetLocalFrameClient()->GetPreviewsStateForFrame() &
      WebURLRequest::kClientLoFiOn) {
    request.AddHTTPHeaderField(
        "Intervention",
        "<https://www.chromestatus.com/features/6072546726248448>; "
        "level=\"warning\"");
  }
}
