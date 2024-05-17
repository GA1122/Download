void GuestViewBase::Destroy() {
  if (is_being_destroyed_)
    return;

  is_being_destroyed_ = true;

  StopTrackingEmbedderZoomLevel();
  owner_web_contents_ = nullptr;

  DCHECK(web_contents());

  WillDestroy();

  weak_ptr_factory_.InvalidateWeakPtrs();

  guest_host_->WillDestroy();
  guest_host_ = nullptr;

  webcontents_guestview_map.Get().erase(web_contents());
  GuestViewManager::FromBrowserContextIfAvailable(browser_context_)->
      RemoveGuest(guest_instance_id_);
  pending_events_.clear();

  delete web_contents();
}
