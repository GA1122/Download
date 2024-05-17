bool BrowsingDataExtensionFunction::RunImpl() {
  if (BrowsingDataRemover::is_removing()) {
    error_ = extension_clear_api_constants::kOneAtATimeError;
    return false;
  }

  double ms_since_epoch;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDouble(0, &ms_since_epoch));
  remove_since_ = (ms_since_epoch == 0) ?
      base::Time::UnixEpoch() :
      base::Time::FromDoubleT(ms_since_epoch / 1000.0);

  removal_mask_ = GetRemovalMask();

  if (removal_mask_ & BrowsingDataRemover::REMOVE_PLUGIN_DATA) {
    Profile* profile = GetCurrentBrowser()->profile();
    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(
            &BrowsingDataExtensionFunction::CheckRemovingPluginDataSupported,
            this,
            make_scoped_refptr(PluginPrefs::GetForProfile(profile))));
  } else {
    StartRemoving();
  }

  return true;
}
