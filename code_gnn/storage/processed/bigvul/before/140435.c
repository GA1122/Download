  NotifyingObserver(ContentSuggestionsService* service,
                    Profile* profile)
      : service_(service),
        profile_(profile),
        app_status_listener_(base::Bind(&NotifyingObserver::AppStatusChanged,
                                        base::Unretained(this))),
        weak_ptr_factory_(this) {}
