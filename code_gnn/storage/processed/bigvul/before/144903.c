void RenderWidgetHostViewAura::ForwardKeyboardEvent(
    const NativeWebKeyboardEvent& event,
    bool* update_event) {
  RenderWidgetHostImpl* target_host = host_;

  if (host_->delegate())
    target_host = host_->delegate()->GetFocusedRenderWidgetHost(host_);
  if (!target_host)
    return;

#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
  ui::TextEditKeyBindingsDelegateAuraLinux* keybinding_delegate =
      ui::GetTextEditKeyBindingsDelegate();
  std::vector<ui::TextEditCommandAuraLinux> commands;
  if (!event.skip_in_browser &&
      keybinding_delegate &&
      event.os_event &&
      keybinding_delegate->MatchEvent(*event.os_event, &commands)) {
    EditCommands edit_commands;
    for (std::vector<ui::TextEditCommandAuraLinux>::const_iterator it =
             commands.begin(); it != commands.end(); ++it) {
      edit_commands.push_back(EditCommand(it->GetCommandString(),
                                          it->argument()));
    }

    target_host->ForwardKeyboardEventWithCommands(event, &edit_commands,
                                                  update_event);
    return;
  }
#endif

  target_host->ForwardKeyboardEventWithCommands(event, nullptr, update_event);
}
