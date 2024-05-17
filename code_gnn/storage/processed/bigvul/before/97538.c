void ProfileSyncComponentsFactoryImpl::RegisterCommonDataTypes(
    ProfileSyncService* pss) {
  if (!command_line_->HasSwitch(switches::kDisableSyncAutofill)) {
    pss->RegisterDataTypeController(
        new AutofillDataTypeController(this, profile_, pss));
  }

  if (!command_line_->HasSwitch(switches::kDisableSyncAutofillProfile)) {
    pss->RegisterDataTypeController(
        new AutofillProfileDataTypeController(this, profile_, pss));
  }

  if (!command_line_->HasSwitch(switches::kDisableSyncBookmarks)) {
    pss->RegisterDataTypeController(
        new BookmarkDataTypeController(this, profile_, pss));
  }

  if (!profile_->GetPrefs()->GetBoolean(prefs::kSavingBrowserHistoryDisabled) &&
      !command_line_->HasSwitch(switches::kDisableSyncTypedUrls)) {
    pss->RegisterDataTypeController(
        new TypedUrlDataTypeController(this, profile_, pss));
  }

  if (command_line_->HasSwitch(switches::kHistoryEnableFullHistorySync) &&
      !command_line_->HasSwitch(
          switches::kDisableSyncHistoryDeleteDirectives)) {
    pss->RegisterDataTypeController(
        new UIDataTypeController(
            syncer::HISTORY_DELETE_DIRECTIVES, this, profile_, pss));
  }

  if (!command_line_->HasSwitch(switches::kDisableSyncTabs)) {
    pss->RegisterDataTypeController(
        new ProxyDataTypeController(syncer::PROXY_TABS));
    pss->RegisterDataTypeController(
        new SessionDataTypeController(this, profile_, pss));
  }

  if (command_line_->HasSwitch(switches::kEnableSyncFavicons)) {
    pss->RegisterDataTypeController(
        new UIDataTypeController(syncer::FAVICON_IMAGES,
                                 this,
                                 profile_,
                                 pss));
    pss->RegisterDataTypeController(
        new UIDataTypeController(syncer::FAVICON_TRACKING,
                                 this,
                                 profile_,
                                 pss));
  }

  if (!command_line_->HasSwitch(switches::kDisableSyncPasswords)) {
    pss->RegisterDataTypeController(
        new PasswordDataTypeController(this, profile_, pss));
  }
}
