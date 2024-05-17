void TranslateManager::OnURLFetchComplete(const URLFetcher* source,
                                          const GURL& url,
                                          const URLRequestStatus& status,
                                          int response_code,
                                          const ResponseCookies& cookies,
                                          const std::string& data) {
  scoped_ptr<const URLFetcher> delete_ptr(source);
  DCHECK(translate_script_request_pending_);
  translate_script_request_pending_ = false;
  bool error =
      (status.status() != URLRequestStatus::SUCCESS || response_code != 200);

  if (!error) {
    base::StringPiece str = ResourceBundle::GetSharedInstance().
        GetRawDataResource(IDR_TRANSLATE_JS);
    DCHECK(translate_script_.empty());
    str.CopyToString(&translate_script_);
    translate_script_ += "\n" + data;
    MessageLoop::current()->PostDelayedTask(FROM_HERE,
        method_factory_.NewRunnableMethod(
            &TranslateManager::ClearTranslateScript),
        translate_script_expiration_delay_);
  }

  std::vector<PendingRequest>::const_iterator iter;
  for (iter = pending_requests_.begin(); iter != pending_requests_.end();
       ++iter) {
    const PendingRequest& request = *iter;
    TabContents* tab = tab_util::GetTabContentsByID(request.render_process_id,
                                                    request.render_view_id);
    if (!tab) {
      continue;
    }
    NavigationEntry* entry = tab->controller().GetActiveEntry();
    if (!entry || entry->page_id() != request.page_id) {
      continue;
    }

    if (error) {
      ShowInfoBar(tab, TranslateInfoBarDelegate::CreateErrorDelegate(
          TranslateErrors::NETWORK, tab,
          request.source_lang, request.target_lang));
    } else {
      DoTranslatePage(tab, translate_script_,
                      request.source_lang, request.target_lang);
    }
  }
  pending_requests_.clear();
}
