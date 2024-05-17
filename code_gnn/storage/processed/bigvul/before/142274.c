void ChromePasswordManagerClient::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  if (!navigation_handle->IsInMainFrame() || !navigation_handle->HasCommitted())
    return;

  if (!navigation_handle->IsSameDocument()) {
    metrics_recorder_.reset();
  }

  if (!navigation_handle->IsSameDocument())
    content_credential_manager_.DisconnectBinding();

#if !defined(OS_ANDROID)
  password_reuse_detection_manager_.DidNavigateMainFrame(GetMainFrameURL());
  AddToWidgetInputEventObservers(
      web_contents()->GetRenderViewHost()->GetWidget(), this);
#else    
  PasswordAccessoryController* accessory =
      PasswordAccessoryController::GetIfExisting(web_contents());
  if (accessory)
    accessory->DidNavigateMainFrame();
#endif   
}
