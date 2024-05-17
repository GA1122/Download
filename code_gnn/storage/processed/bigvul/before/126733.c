void BrowserView::Cut() {
  if (!DoCutCopyPaste(&content::RenderWidgetHost::Cut)) {
    ui_controls::SendKeyPress(GetNativeWindow(), ui::VKEY_X,
                              true, false, false, false);
  }
}
