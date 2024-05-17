content::WebContents* GuestViewBase::CreateNewGuestWindow(
    const content::WebContents::CreateParams& create_params) {
  auto guest_manager =
      GuestViewManager::FromBrowserContextIfAvailable(browser_context());
  return guest_manager->CreateGuestWithWebContentsParams(
      GetViewType(),
      owner_web_contents(),
      create_params);
}
