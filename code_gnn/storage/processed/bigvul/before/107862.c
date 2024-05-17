bool BlockedPlugin::OnMessageReceived(const IPC::Message& message) {
  if (message.type() == ViewMsg_CustomContextMenuAction::ID &&
      gLastActiveMenu == this) {
    ViewMsg_CustomContextMenuAction::Dispatch(
        &message, this, this, &BlockedPlugin::OnMenuItemSelected);
  } else if (message.type() == ViewMsg_LoadBlockedPlugins::ID) {
    LoadPlugin();
  } else if (message.type() == ViewMsg_DisplayPrerenderedPage::ID) {
    if (is_blocked_for_prerendering_)
      LoadPlugin();
  }

  return false;
}
