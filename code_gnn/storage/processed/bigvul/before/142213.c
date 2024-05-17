    static void RegisterJSONConverter(
        base::JSONValueConverter<EntryFolderFeature>* converter) {
      converter->RegisterBoolField("isMachineRoot",
                                   &EntryFolderFeature::is_machine_root);
      converter->RegisterBoolField(
          "isArbitrarySyncFolder",
          &EntryFolderFeature::is_arbitrary_sync_folder);
      converter->RegisterBoolField("isExternalMedia",
                                   &EntryFolderFeature::is_external_media);
    }
