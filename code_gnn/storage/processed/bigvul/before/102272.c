void ExtensionPrefs::Init(bool extensions_disabled) {
  MakePathsRelative();

  InitPrefStore(extensions_disabled);

  content_settings_store_->AddObserver(this);
}
