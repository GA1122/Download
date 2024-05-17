void ExtensionViewGuest::ApplyAttributes(const base::DictionaryValue& params) {
  std::string src;
  params.GetString(extensionview::kAttributeSrc, &src);
  NavigateGuest(src, false  );
}
