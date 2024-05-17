void TestingAutomationProvider::GetRedirectsFrom(int tab_handle,
                                                 const GURL& source_url,
                                                 IPC::Message* reply_message) {
  if (redirect_query_) {
    LOG(ERROR) << "Can only handle one redirect query at once.";
  } else if (tab_tracker_->ContainsHandle(tab_handle)) {
    NavigationController* tab = tab_tracker_->GetResource(tab_handle);
    Profile* profile = Profile::FromBrowserContext(tab->GetBrowserContext());
    HistoryService* history_service =
        profile->GetHistoryService(Profile::EXPLICIT_ACCESS);

    DCHECK(history_service) << "Tab " << tab_handle << "'s profile " <<
                               "has no history service";
    if (history_service) {
      DCHECK(!reply_message_);
      reply_message_ = reply_message;
      redirect_query_ = history_service->QueryRedirectsFrom(
          source_url, &consumer_,
          base::Bind(&TestingAutomationProvider::OnRedirectQueryComplete,
                     base::Unretained(this)));
      return;   
    }
  }

  std::vector<GURL> empty;
  AutomationMsg_RedirectsFrom::WriteReplyParams(reply_message, false, empty);
  Send(reply_message);
}
