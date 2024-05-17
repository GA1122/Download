void GuestViewBase::InitWithWebContents(
    const base::DictionaryValue& create_params,
    content::WebContents* guest_web_contents) {
  DCHECK(guest_web_contents);

  ui_zoom::ZoomController::CreateForWebContents(guest_web_contents);

  owner_contents_observer_.reset(
      new OwnerContentsObserver(this, owner_web_contents_));

  WebContentsObserver::Observe(guest_web_contents);
  guest_web_contents->SetDelegate(this);
  webcontents_guestview_map.Get().insert(
      std::make_pair(guest_web_contents, this));
  GuestViewManager::FromBrowserContextIfAvailable(browser_context_)->
      AddGuest(guest_instance_id_, guest_web_contents);

  create_params.GetInteger(guestview::kParameterInstanceId,
                           &view_instance_id_);

  if (CanRunInDetachedState())
    SetUpSizing(create_params);

  auto zoom_controller =
      ui_zoom::ZoomController::FromWebContents(web_contents());
  zoom_controller->AddObserver(this);

  DidInitialize(create_params);
}
