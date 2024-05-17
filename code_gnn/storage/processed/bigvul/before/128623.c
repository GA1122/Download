bool ContextualSearchDelegate::CanSendPageURL(
    const GURL& current_page_url,
    Profile* profile,
    TemplateURLService* template_url_service) {
  if (field_trial_->IsSendBasePageURLDisabled())
    return false;

  TemplateURL* default_search_provider =
      template_url_service->GetDefaultSearchProvider();
  bool is_default_search_provider_google =
      default_search_provider &&
      default_search_provider->url_ref().HasGoogleBaseURLs(
          template_url_service->search_terms_data());
  if (!is_default_search_provider_google)
    return false;

  if (current_page_url.scheme() != url::kHttpScheme &&
      (current_page_url.scheme() != url::kHttpsScheme))
    return false;

  browser_sync::ProfileSyncService* service =
      ProfileSyncServiceFactory::GetInstance()->GetForProfile(profile);
  sync_driver::SyncPrefs sync_prefs(profile->GetPrefs());
  if (service == NULL || !service->CanSyncStart() ||
      !sync_prefs.GetPreferredDataTypes(syncer::UserTypes())
           .Has(syncer::PROXY_TABS) ||
      !service->GetActiveDataTypes().Has(syncer::HISTORY_DELETE_DIRECTIVES)) {
    return false;
  }

  return true;
}
