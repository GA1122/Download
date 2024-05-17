ExtensionContextMenuModel::ExtensionContextMenuModel(
    const Extension* extension,
    Browser* browser,
    PopupDelegate* delegate)
    : ALLOW_THIS_IN_INITIALIZER_LIST(SimpleMenuModel(this)),
      extension_id_(extension->id()),
      browser_(browser),
      profile_(browser->profile()),
      delegate_(delegate) {
  extension_action_ = extension->browser_action();
  if (!extension_action_)
    extension_action_ = extension->page_action();

  InitCommonCommands();

  if (profile_->GetPrefs()->GetBoolean(prefs::kExtensionsUIDeveloperMode) &&
      delegate_) {
    AddSeparator();
    AddItemWithStringId(INSPECT_POPUP, IDS_EXTENSION_ACTION_INSPECT_POPUP);
  }
}
