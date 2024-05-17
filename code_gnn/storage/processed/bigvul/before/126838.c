void BrowserView::Paste() {
  if (!DoCutCopyPaste(&content::RenderWidgetHost::Paste)) {
    ui_controls::SendKeyPress(GetNativeWindow(), ui::VKEY_V,
                              true, false, false, false);
  }
}
