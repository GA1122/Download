void ExtensionInstallPrompt::Prompt::SetWebstoreData(
    const std::string& localized_user_count,
    bool show_user_count,
    double average_rating,
    int rating_count) {
  CHECK(AllowWebstoreData(type_));
  localized_user_count_ = localized_user_count;
  show_user_count_ = show_user_count;
  average_rating_ = average_rating;
  rating_count_ = rating_count;
  has_webstore_data_ = true;
}
