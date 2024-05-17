    static void RegisterJSONConverter(
        base::JSONValueConverter<TestEntryInfo>* converter) {
      converter->RegisterCustomField("type", &TestEntryInfo::type,
                                     &MapStringToEntryType);
      converter->RegisterStringField("sourceFileName",
                                     &TestEntryInfo::source_file_name);
      converter->RegisterStringField("targetPath", &TestEntryInfo::target_path);
      converter->RegisterStringField("nameText", &TestEntryInfo::name_text);
      converter->RegisterStringField("teamDriveName",
                                     &TestEntryInfo::team_drive_name);
      converter->RegisterStringField("computerName",
                                     &TestEntryInfo::computer_name);
      converter->RegisterStringField("mimeType", &TestEntryInfo::mime_type);
      converter->RegisterCustomField("sharedOption",
                                     &TestEntryInfo::shared_option,
                                     &MapStringToSharedOption);
      converter->RegisterCustomField("lastModifiedTime",
                                     &TestEntryInfo::last_modified_time,
                                     &MapStringToTime);
      converter->RegisterNestedField("capabilities",
                                     &TestEntryInfo::capabilities);
      converter->RegisterNestedField("folderFeature",
                                     &TestEntryInfo::folder_feature);
      converter->RegisterBoolField("pinned", &TestEntryInfo::pinned);
    }
