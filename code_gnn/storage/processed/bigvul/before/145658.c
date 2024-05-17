void ExtensionOptionsGuest::GuestViewDidStopLoading() {
  scoped_ptr<base::DictionaryValue> args(new base::DictionaryValue());
  DispatchEventToView(make_scoped_ptr(new GuestViewEvent(
      extension_options_internal::OnLoad::kEventName, std::move(args))));
}
