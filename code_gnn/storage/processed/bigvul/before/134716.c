void GuestViewBase::DidDetach() {
  GuestViewManager::FromBrowserContextIfAvailable(browser_context_)->
      DetachGuest(this);
  StopTrackingEmbedderZoomLevel();
  owner_web_contents()->Send(new GuestViewMsg_GuestDetached(
      element_instance_id_));
  element_instance_id_ = guestview::kInstanceIDNone;
}
