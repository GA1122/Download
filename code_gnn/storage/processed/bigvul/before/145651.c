void ExtensionOptionsGuest::CloseContents(WebContents* source) {
  DispatchEventToView(make_scoped_ptr(
      new GuestViewEvent(extension_options_internal::OnClose::kEventName,
                         make_scoped_ptr(new base::DictionaryValue()))));
}
