void ExtensionService::LoadAllExtensions() {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  base::TimeTicks start_time = base::TimeTicks::Now();

  LoadComponentExtensions();

  scoped_ptr<ExtensionPrefs::ExtensionsInfo> extensions_info(
      extension_prefs_->GetInstalledExtensionsInfo());

  std::vector<int> reload_reason_counts(NUM_MANIFEST_RELOAD_REASONS, 0);
  bool should_write_prefs = false;

  for (size_t i = 0; i < extensions_info->size(); ++i) {
    ExtensionInfo* info = extensions_info->at(i).get();

    ManifestReloadReason reload_reason = ShouldReloadExtensionManifest(*info);
    ++reload_reason_counts[reload_reason];
    UMA_HISTOGRAM_ENUMERATION("Extensions.ManifestReloadEnumValue",
                              reload_reason, 100);

    if (reload_reason != NOT_NEEDED) {
      base::ThreadRestrictions::ScopedAllowIO allow_io;

      int flags = Extension::NO_FLAGS;
      if (Extension::ShouldDoStrictErrorChecking(info->extension_location))
        flags |= Extension::STRICT_ERROR_CHECKS;
      if (extension_prefs_->AllowFileAccess(info->extension_id))
        flags |= Extension::ALLOW_FILE_ACCESS;
      std::string error;
      scoped_refptr<const Extension> extension(
          extension_file_util::LoadExtension(
              info->extension_path,
              info->extension_location,
              flags,
              &error));

      if (extension.get()) {
        extensions_info->at(i)->extension_manifest.reset(
            static_cast<DictionaryValue*>(
                extension->manifest_value()->DeepCopy()));
        should_write_prefs = true;
      }
    }
  }

  for (size_t i = 0; i < extensions_info->size(); ++i) {
    LoadInstalledExtension(*extensions_info->at(i), should_write_prefs);
  }

  OnLoadedInstalledExtensions();

  UMA_HISTOGRAM_COUNTS_100("Extensions.ManifestReloadNotNeeded",
                           reload_reason_counts[NOT_NEEDED]);
  UMA_HISTOGRAM_COUNTS_100("Extensions.ManifestReloadUnpackedDir",
                           reload_reason_counts[UNPACKED_DIR]);
  UMA_HISTOGRAM_COUNTS_100("Extensions.ManifestReloadNeedsRelocalization",
                           reload_reason_counts[NEEDS_RELOCALIZATION]);

  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadAll", extensions_.size());
  UMA_HISTOGRAM_COUNTS_100("Extensions.Disabled", disabled_extensions_.size());

  UMA_HISTOGRAM_TIMES("Extensions.LoadAllTime",
                      base::TimeTicks::Now() - start_time);

  int app_count = 0;
  int hosted_app_count = 0;
  int packaged_app_count = 0;
  int user_script_count = 0;
  int extension_count = 0;
  int theme_count = 0;
  int external_count = 0;
  int page_action_count = 0;
  int browser_action_count = 0;
  ExtensionList::iterator ex;
  for (ex = extensions_.begin(); ex != extensions_.end(); ++ex) {
    Extension::Location location = (*ex)->location();
    Extension::Type type = (*ex)->GetType();
    if ((*ex)->is_app()) {
      UMA_HISTOGRAM_ENUMERATION("Extensions.AppLocation",
                                location, 100);
    } else if (type == Extension::TYPE_EXTENSION) {
      UMA_HISTOGRAM_ENUMERATION("Extensions.ExtensionLocation",
                                location, 100);
    }

    if (location == Extension::COMPONENT)
      continue;

    if (location == Extension::LOAD)
      continue;

    UMA_HISTOGRAM_ENUMERATION("Extensions.LoadType", type, 100);
    switch (type) {
      case Extension::TYPE_THEME:
        ++theme_count;
        break;
      case Extension::TYPE_USER_SCRIPT:
        ++user_script_count;
        break;
      case Extension::TYPE_HOSTED_APP:
        ++app_count;
        ++hosted_app_count;
        break;
      case Extension::TYPE_PACKAGED_APP:
        ++app_count;
        ++packaged_app_count;
        break;
      case Extension::TYPE_EXTENSION:
      default:
        ++extension_count;
        break;
    }
    if (Extension::IsExternalLocation(location))
      ++external_count;
    if ((*ex)->page_action() != NULL)
      ++page_action_count;
    if ((*ex)->browser_action() != NULL)
      ++browser_action_count;

    RecordPermissionMessagesHistogram(
        ex->get(), "Extensions.Permissions_Load");
  }
  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadApp", app_count);
  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadHostedApp", hosted_app_count);
  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadPackagedApp", packaged_app_count);
  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadExtension", extension_count);
  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadUserScript", user_script_count);
  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadTheme", theme_count);
  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadExternal", external_count);
  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadPageAction", page_action_count);
  UMA_HISTOGRAM_COUNTS_100("Extensions.LoadBrowserAction",
                           browser_action_count);
}
