void WebLocalFrameImpl::StartReload(WebFrameLoadType frame_load_type) {
  DCHECK(GetFrame());
  DCHECK(IsReloadLoadType(frame_load_type));
  ResourceRequest request =
      GetFrame()->Loader().ResourceRequestForReload(frame_load_type);
  if (request.IsNull())
    return;
  request.SetRequestorOrigin(GetFrame()->GetDocument()->GetSecurityOrigin());
  if (GetTextFinder())
    GetTextFinder()->ClearActiveFindMatch();

  GetFrame()->Loader().StartNavigation(
      FrameLoadRequest(nullptr, request,  AtomicString(),
                       kCheckContentSecurityPolicy,
                       base::UnguessableToken::Create()),
      frame_load_type);
}
