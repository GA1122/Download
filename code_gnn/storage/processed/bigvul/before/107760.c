bool FrameLoader::PrepareRequestForThisFrame(FrameLoadRequest& request) {
  if (!request.OriginDocument())
    return true;

  KURL url = request.GetResourceRequest().Url();
  if (url.ProtocolIsJavaScript()) {
    Document* origin_document = request.OriginDocument();
    bool javascript_url_is_allowed =
        request.ShouldCheckMainWorldContentSecurityPolicy() ==
            kDoNotCheckContentSecurityPolicy ||
        origin_document->GetContentSecurityPolicy()->AllowInline(
            ContentSecurityPolicy::InlineType::kJavaScriptURL,
            frame_->DeprecatedLocalOwner(), url.GetString(),
            String()  , origin_document->Url(),
            OrdinalNumber::First());

    if (!javascript_url_is_allowed)
      return false;

    if (frame_->Owner() && frame_->Owner()->GetSandboxFlags() & kSandboxOrigin)
      return false;

    frame_->GetDocument()->ProcessJavaScriptUrl(
        url, request.ShouldCheckMainWorldContentSecurityPolicy());
    return false;
  }

  if (!request.OriginDocument()->GetSecurityOrigin()->CanDisplay(url)) {
    request.OriginDocument()->AddConsoleMessage(ConsoleMessage::Create(
        kSecurityMessageSource, mojom::ConsoleMessageLevel::kError,
        "Not allowed to load local resource: " + url.ElidedString()));
    return false;
  }

  if (frame_->IsMainFrame() &&
      !frame_->Client()->AllowContentInitiatedDataUrlNavigations(
          request.OriginDocument()->Url()) &&
      (url.ProtocolIs("filesystem") ||
       (url.ProtocolIsData() &&
        network_utils::IsDataURLMimeTypeSupported(url)))) {
    frame_->GetDocument()->AddConsoleMessage(ConsoleMessage::Create(
        kSecurityMessageSource, mojom::ConsoleMessageLevel::kError,
        "Not allowed to navigate top frame to " + url.Protocol() +
            " URL: " + url.ElidedString()));
    return false;
  }

  if (!request.Form() && request.FrameName().IsEmpty())
    request.SetFrameName(frame_->GetDocument()->BaseTarget());
  return true;
}
