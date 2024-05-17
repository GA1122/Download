void TranslateManager::DoTranslatePage(TabContents* tab,
                                       const std::string& translate_script,
                                       const std::string& source_lang,
                                       const std::string& target_lang) {
  NavigationEntry* entry = tab->controller().GetActiveEntry();
  if (!entry) {
    NOTREACHED();
    return;
  }

  tab->language_state().set_translation_pending(true);
  tab->render_view_host()->TranslatePage(entry->page_id(), translate_script,
                                         source_lang, target_lang);
}
