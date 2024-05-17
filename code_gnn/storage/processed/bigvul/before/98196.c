void TranslateManager::TranslatePage(TabContents* tab_contents,
                                      const std::string& source_lang,
                                      const std::string& target_lang) {
  NavigationEntry* entry = tab_contents->controller().GetActiveEntry();
  if (!entry) {
    NOTREACHED();
    return;
  }

  TranslateInfoBarDelegate* infobar = TranslateInfoBarDelegate::CreateDelegate(
      TranslateInfoBarDelegate::TRANSLATING, tab_contents,
      source_lang, target_lang);
  if (!infobar) {
    NOTREACHED();
    return;
  }
  ShowInfoBar(tab_contents, infobar);

  if (!translate_script_.empty()) {
    DoTranslatePage(tab_contents, translate_script_, source_lang, target_lang);
    return;
  }

  RenderViewHost* rvh = tab_contents->render_view_host();
  PendingRequest request;
  request.render_process_id = rvh->process()->id();
  request.render_view_id = rvh->routing_id();
  request.page_id = entry->page_id();
  request.source_lang = source_lang;
  request.target_lang = target_lang;
  pending_requests_.push_back(request);
  RequestTranslateScript();
}
