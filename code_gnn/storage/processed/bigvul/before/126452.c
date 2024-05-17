void BrowserWindowGtk::UpdateToolbar(TabContents* contents,
                                     bool should_restore_state) {
  TRACE_EVENT0("ui::gtk", "BrowserWindowGtk::UpdateToolbar");
  toolbar_->UpdateWebContents(contents->web_contents(), should_restore_state);
}
