 WebstoreStandaloneInstaller::WebstoreStandaloneInstaller(
     const std::string& webstore_item_id,
     Profile* profile,
    const Callback& callback)
    : id_(webstore_item_id),
      callback_(callback),
      profile_(profile),
      install_source_(WebstoreInstaller::INSTALL_SOURCE_INLINE),
      show_user_count_(true),
      average_rating_(0.0),
      rating_count_(0) {
}
