  static void RegisterJSONConverter(
      base::JSONValueConverter<ExpectFileTasksMessage>* converter) {
    converter->RegisterCustomField(
        "openType", &ExpectFileTasksMessage::open_type, &MapStringToOpenType);
    converter->RegisterRepeatedString("fileNames",
                                      &ExpectFileTasksMessage::file_names);
  }
