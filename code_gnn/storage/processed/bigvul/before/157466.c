  void AddRegistryKeyWatcher(const wchar_t* key_path) {
    DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

    auto reg_key = std::make_unique<base::win::RegKey>(HKEY_CURRENT_USER,
                                                       key_path, KEY_NOTIFY);

    if (reg_key->Valid() &&
        reg_key->StartWatching(
            base::Bind(&OpenSystemSettingsHelper::OnRegistryKeyChanged,
                       weak_ptr_factory_.GetWeakPtr()))) {
      registry_key_watchers_.push_back(std::move(reg_key));
    }
  }
