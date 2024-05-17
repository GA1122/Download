bool FrameLoader::PrepareRequestForThisFrame(FrameLoadRequest& request) {
  if (!request.OriginDocument())
    return true;

  KURL url = request.GetResourceRequest().Url();
  if (frame_->GetScriptController().ExecuteScriptIfJavaScriptURL(url, nullptr))
    return false;

  if (!request.OriginDocument()->GetSecurityOrigin()->CanDisplay(url)) {
    request.OriginDocument()->AddConsoleMessage(ConsoleMessage::Create(
        kSecurityMessageSource, kErrorMessageLevel,
        "Not allowed to load local resource: " + url.ElidedString()));
    return false;
  }

  if (frame_->IsMainFrame() &&
      !request.GetResourceRequest().IsSameDocumentNavigation() &&
      !frame_->Client()->AllowContentInitiatedDataUrlNavigations(
          request.OriginDocument()->Url()) &&
      !request.GetResourceRequest().GetSuggestedFilename().has_value() &&
      url.ProtocolIsData() && NetworkUtils::IsDataURLMimeTypeSupported(url)) {
    frame_->GetDocument()->AddConsoleMessage(ConsoleMessage::Create(
        kSecurityMessageSource, kErrorMessageLevel,
        "Not allowed to navigate top frame to data URL: " +
            url.ElidedString()));
    return false;
  }

  if (!request.Form() && request.FrameName().IsEmpty())
    request.SetFrameName(frame_->GetDocument()->BaseTarget());
  return true;
}
