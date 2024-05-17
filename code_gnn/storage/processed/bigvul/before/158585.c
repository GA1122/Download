void WebLocalFrameImpl::StartNavigation(const WebURLRequest& request) {
  DCHECK(GetFrame());
  DCHECK(!request.IsNull());
  DCHECK(!request.Url().ProtocolIs("javascript"));
  if (GetTextFinder())
    GetTextFinder()->ClearActiveFindMatch();

  GetFrame()->Loader().StartNavigation(
      FrameLoadRequest(
          nullptr, request.ToResourceRequest(),  AtomicString(),
          kCheckContentSecurityPolicy, base::UnguessableToken::Create()),
      WebFrameLoadType::kStandard);
}
