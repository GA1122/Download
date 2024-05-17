GuestViewBase::GuestViewBase(content::WebContents* owner_web_contents)
    : owner_web_contents_(owner_web_contents),
      browser_context_(owner_web_contents->GetBrowserContext()),
      guest_instance_id_(
          GuestViewManager::FromBrowserContextIfAvailable(browser_context_)->
              GetNextInstanceID()),
      view_instance_id_(guestview::kInstanceIDNone),
      element_instance_id_(guestview::kInstanceIDNone),
      initialized_(false),
      is_being_destroyed_(false),
      guest_host_(nullptr),
      auto_size_enabled_(false),
      is_full_page_plugin_(false),
      guest_proxy_routing_id_(MSG_ROUTING_NONE),
      weak_ptr_factory_(this) {
}
