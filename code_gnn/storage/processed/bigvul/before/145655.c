ExtensionOptionsGuest::ExtensionOptionsGuest(WebContents* owner_web_contents)
    : GuestView<ExtensionOptionsGuest>(owner_web_contents),
      extension_options_guest_delegate_(
          extensions::ExtensionsAPIClient::Get()
              ->CreateExtensionOptionsGuestDelegate(this)) {}
