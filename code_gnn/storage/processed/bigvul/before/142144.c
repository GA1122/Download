  static bool ConvertJSONValue(const base::DictionaryValue& value,
                               ExpectFileTasksMessage* message) {
    base::JSONValueConverter<ExpectFileTasksMessage> converter;
    return converter.Convert(value, message);
  }
