GuestViewBase* GuestViewBase::From(int owner_process_id,
                                   int guest_instance_id) {
  auto host = content::RenderProcessHost::FromID(owner_process_id);
  if (!host)
    return nullptr;

  content::WebContents* guest_web_contents =
      GuestViewManager::FromBrowserContextIfAvailable(
          host->GetBrowserContext())->
              GetGuestByInstanceIDSafely(guest_instance_id, owner_process_id);
  if (!guest_web_contents)
    return nullptr;

  return GuestViewBase::FromWebContents(guest_web_contents);
}
