void RenderWidgetHostViewAura::OnTextSelectionChanged(
    TextInputManager* text_input_manager,
    RenderWidgetHostViewBase* updated_view) {
#if defined(USE_X11) && !defined(OS_CHROMEOS)
  if (!GetTextInputManager())
    return;

  RenderWidgetHostViewBase* focused_view =
      is_guest_view_hack_ ? this : GetFocusedWidget()
                                       ? GetFocusedWidget()->GetView()
                                       : nullptr;

  if (!focused_view)
    return;

  const TextInputManager::TextSelection* selection =
      GetTextInputManager()->GetTextSelection(focused_view);
  if (selection->selected_text().length()) {
    ui::ScopedClipboardWriter clipboard_writer(ui::CLIPBOARD_TYPE_SELECTION);
    clipboard_writer.WriteText(selection->selected_text());
  }
#endif   
}
