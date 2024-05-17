void BrowserView::Copy() {
  if (!DoCutCopyPaste(&content::RenderWidgetHost::Copy)) {
    ui_controls::SendKeyPress(GetNativeWindow(), ui::VKEY_C,
                              true, false, false, false);
  }
}
