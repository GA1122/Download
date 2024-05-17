void ChromeExtensionsAPIClient::SaveImageDataToClipboard(
    const std::vector<char>& image_data,
    api::clipboard::ImageType type,
    AdditionalDataItemList additional_items,
    const base::Closure& success_callback,
    const base::Callback<void(const std::string&)>& error_callback) {
  if (!clipboard_extension_helper_)
    clipboard_extension_helper_ = base::MakeUnique<ClipboardExtensionHelper>();
  clipboard_extension_helper_->DecodeAndSaveImageData(
      image_data, type, std::move(additional_items), success_callback,
      error_callback);
}
