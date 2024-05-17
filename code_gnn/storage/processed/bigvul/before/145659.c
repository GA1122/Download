bool ExtensionOptionsGuest::HandleContextMenu(
    const content::ContextMenuParams& params) {
  if (!extension_options_guest_delegate_)
    return false;

  return extension_options_guest_delegate_->HandleContextMenu(params);
}
