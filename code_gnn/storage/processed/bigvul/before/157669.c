void ImeObserver::OnActivate(const std::string& component_id) {
  if (extension_id_.empty())
    return;

  std::unique_ptr<base::ListValue> args(input_ime::OnActivate::Create(
      component_id, input_ime::ParseScreenType(GetCurrentScreenType())));

  DispatchEventToExtension(extensions::events::INPUT_IME_ON_ACTIVATE,
                           input_ime::OnActivate::kEventName,
                           std::move(args));
}
