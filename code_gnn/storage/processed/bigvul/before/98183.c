void TranslateManager::InitiateTranslationPosted(
    int process_id, int render_id, const std::string& page_lang) {
  TabContents* tab = tab_util::GetTabContentsByID(process_id, render_id);
  if (!tab || tab->language_state().translation_pending())
    return;

  InitiateTranslation(tab, page_lang);
}
