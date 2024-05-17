GuestViewBase* ExtensionOptionsGuest::Create(WebContents* owner_web_contents) {
  return new ExtensionOptionsGuest(owner_web_contents);
}
