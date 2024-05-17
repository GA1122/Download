  SingleDriveEntryPropertiesGetter(const base::FilePath local_path,
                                   linked_ptr<DriveEntryProperties> properties,
                                   Profile* const profile,
                                   const ResultCallback& callback)
      : callback_(callback),
        local_path_(local_path),
        properties_(properties),
        running_profile_(profile),
        file_owner_profile_(NULL),
        weak_ptr_factory_(this) {
    DCHECK(!callback_.is_null());
    DCHECK(profile);
  }
