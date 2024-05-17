PeopleHandler::PeopleHandler(Profile* profile)
    : profile_(profile),
      configuring_sync_(false),
      identity_manager_observer_(this),
#if BUILDFLAG(ENABLE_DICE_SUPPORT)
      sync_service_observer_(this),
      account_tracker_observer_(this) {
}
