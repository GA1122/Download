void AutomationProviderSearchEngineObserver::OnTemplateURLServiceChanged() {
  if (provider_) {
    TemplateURLService* url_service =
        TemplateURLServiceFactory::GetForProfile(profile_);
    url_service->RemoveObserver(this);
    AutomationJSONReply(provider_, reply_message_.release()).SendSuccess(NULL);
  }
  delete this;
}
