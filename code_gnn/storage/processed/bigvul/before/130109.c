base::DictionaryValue* ParseDistributionPreferences(
    const std::string& json_data) {
  JSONStringValueDeserializer json(json_data);
  std::string error;
  scoped_ptr<base::Value> root(json.Deserialize(NULL, &error));
  if (!root.get()) {
    LOG(WARNING) << "Failed to parse master prefs file: " << error;
    return NULL;
  }
  if (!root->IsType(base::Value::TYPE_DICTIONARY)) {
    LOG(WARNING) << "Failed to parse master prefs file: "
                 << "Root item must be a dictionary.";
    return NULL;
  }
  return static_cast<base::DictionaryValue*>(root.release());
}
