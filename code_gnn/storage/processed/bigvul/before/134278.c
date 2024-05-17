void OmniboxViewViews::OnBlur() {
  saved_selection_for_focus_change_ = GetSelectedRange();

  views::Textfield::OnBlur();
  gfx::NativeView native_view = NULL;
#if defined(USE_AURA)
  views::Widget* widget = GetWidget();
  if (widget) {
    aura::client::FocusClient* client =
        aura::client::GetFocusClient(widget->GetNativeView());
    if (client)
      native_view = client->GetFocusedWindow();
  }
#endif
  model()->OnWillKillFocus(native_view);
  CloseOmniboxPopup();

  model()->OnKillFocus();

  SelectRange(gfx::Range(0));
}
