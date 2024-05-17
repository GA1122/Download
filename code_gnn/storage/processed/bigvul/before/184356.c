 PromoResourceService::PromoResourceService(Profile* profile)
     : WebResourceService(profile->GetPrefs(),
                          GetPromoResourceURL(),
                          true,   
                          prefs::kNtpPromoResourceCacheUpdate,
                          kStartResourceFetchDelay,
                           GetCacheUpdateDelay()),
                           profile_(profile),
                           ALLOW_THIS_IN_INITIALIZER_LIST(
                             weak_ptr_factory_(this)),
                         web_resource_update_scheduled_(false) {
//                              weak_ptr_factory_(this)) {
    ScheduleNotificationOnInit();
  }