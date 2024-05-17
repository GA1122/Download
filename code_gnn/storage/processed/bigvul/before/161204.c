void OnMimeTypesCollected(
    std::vector<std::string>* output,
    std::unique_ptr<std::vector<std::string>> mime_types) {
  *output = *mime_types;
}
