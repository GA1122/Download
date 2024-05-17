WebstoreStandaloneInstaller::GetLocalizedExtensionForDisplay() {
  if (!localized_extension_for_display_.get()) {
    DCHECK(manifest_.get());
    if (!manifest_.get())
      return NULL;

    std::string error;
    localized_extension_for_display_ =
        ExtensionInstallPrompt::GetLocalizedExtensionForDisplay(
            manifest_.get(),
            Extension::REQUIRE_KEY | Extension::FROM_WEBSTORE,
            id_,
            localized_name_,
            localized_description_,
            &error);
  }
  return localized_extension_for_display_.get();
}
