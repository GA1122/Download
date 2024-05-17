NTPInfoObserver::NTPInfoObserver(AutomationProvider* automation,
                                 IPC::Message* reply_message)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      request_(0),
      ntp_info_(new DictionaryValue) {
  top_sites_ = automation_->profile()->GetTopSites();
  if (!top_sites_) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendError("Profile does not have service for querying the top sites.");
    return;
  }
  TabRestoreService* service =
      TabRestoreServiceFactory::GetForProfile(automation_->profile());
  if (!service) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendError("No TabRestoreService.");
    return;
  }

  ExtensionService* ext_service = automation_->profile()->GetExtensionService();
  if (!ext_service) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendError("No ExtensionService.");
    return;
  }
  ListValue* apps_list = new ListValue();
  const ExtensionSet* extensions = ext_service->extensions();
  std::vector<DictionaryValue*>* enabled_apps = GetAppInfoFromExtensions(
      extensions, ext_service);
  for (std::vector<DictionaryValue*>::const_iterator app =
       enabled_apps->begin(); app != enabled_apps->end(); ++app) {
    (*app)->SetBoolean("is_disabled", false);
    apps_list->Append(*app);
  }
  delete enabled_apps;
  const ExtensionSet* disabled_extensions = ext_service->disabled_extensions();
  std::vector<DictionaryValue*>* disabled_apps = GetAppInfoFromExtensions(
      disabled_extensions, ext_service);
  for (std::vector<DictionaryValue*>::const_iterator app =
       disabled_apps->begin(); app != disabled_apps->end(); ++app) {
    (*app)->SetBoolean("is_disabled", true);
    apps_list->Append(*app);
  }
  delete disabled_apps;
  const ExtensionSet* terminated_extensions =
      ext_service->terminated_extensions();
  std::vector<DictionaryValue*>* terminated_apps = GetAppInfoFromExtensions(
      terminated_extensions, ext_service);
  for (std::vector<DictionaryValue*>::const_iterator app =
       terminated_apps->begin(); app != terminated_apps->end(); ++app) {
    (*app)->SetBoolean("is_disabled", true);
    apps_list->Append(*app);
  }
  delete terminated_apps;
  ntp_info_->Set("apps", apps_list);

  ListValue* recently_closed_list = new ListValue;
  RecentlyClosedTabsHandler::CreateRecentlyClosedValues(service->entries(),
                                                        recently_closed_list);
  ntp_info_->Set("recently_closed", recently_closed_list);

  ListValue* default_sites_list = new ListValue;
  history::MostVisitedURLList urls = top_sites_->GetPrepopulatePages();
  for (size_t i = 0; i < urls.size(); ++i) {
    default_sites_list->Append(Value::CreateStringValue(
        urls[i].url.possibly_invalid_spec()));
  }
  ntp_info_->Set("default_sites", default_sites_list);

  registrar_.Add(this, chrome::NOTIFICATION_TOP_SITES_UPDATED,
                 content::Source<history::TopSites>(top_sites_));
  if (top_sites_->loaded()) {
    OnTopSitesLoaded();
  } else {
    registrar_.Add(this, chrome::NOTIFICATION_TOP_SITES_LOADED,
                   content::Source<Profile>(automation_->profile()));
  }
}
