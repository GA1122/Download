void TranslateManager::RevertTranslation(TabContents* tab_contents) {
  NavigationEntry* entry = tab_contents->controller().GetActiveEntry();
  if (!entry) {
    NOTREACHED();
    return;
  }
  tab_contents->render_view_host()->RevertTranslation(entry->page_id());
  tab_contents->language_state().set_current_language(
      tab_contents->language_state().original_language());
}
