  static void Start(const base::FilePath local_path,
                    linked_ptr<DriveEntryProperties> properties,
                    Profile* const profile,
                    const ResultCallback& callback) {

    SingleDriveEntryPropertiesGetter* instance =
        new SingleDriveEntryPropertiesGetter(
            local_path, properties, profile, callback);
    instance->StartProcess();

  }
