  OpenSystemSettingsHelper(const wchar_t* const protocols[],
                           const base::Closure& on_finished_callback)
      : scoped_user_protocol_entry_(protocols[0]),
        on_finished_callback_(on_finished_callback),
        weak_ptr_factory_(this) {
    static const wchar_t kUrlAssociationFormat[] =
        L"SOFTWARE\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\"
        L"%ls\\UserChoice";

    start_time_ = base::TimeTicks::Now();

    for (const wchar_t* const* scan = &protocols[0]; *scan != nullptr; ++scan) {
      AddRegistryKeyWatcher(
          base::StringPrintf(kUrlAssociationFormat, *scan).c_str());
    }
    registry_watcher_count_ = registry_key_watchers_.size();

    timer_.Start(
        FROM_HERE, base::TimeDelta::FromMinutes(2),
        base::Bind(&OpenSystemSettingsHelper::ConcludeInteraction,
                   weak_ptr_factory_.GetWeakPtr(), ConcludeReason::TIMEOUT));
  }
