void DownloadRequestLimiter::TabDownloadState::PromptUserForDownload(
    const DownloadRequestLimiter::Callback& callback) {
  callbacks_.push_back(callback);
  DCHECK(web_contents_);
  if (is_showing_prompt())
    return;

  if (vr::VrTabHelper::IsUiSuppressedInVr(
          web_contents_, vr::UiSuppressedElement::kDownloadPermission)) {
    Cancel();
    return;
  }

  PermissionRequestManager* permission_request_manager =
      PermissionRequestManager::FromWebContents(web_contents_);
  if (permission_request_manager) {
    permission_request_manager->AddRequest(
        new DownloadPermissionRequest(factory_.GetWeakPtr()));
  } else {
    Cancel();
  }
}
