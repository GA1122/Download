bool Extension::ParsePermissions(const char* key,
                                 string16* error,
                                 APIPermissionSet* api_permissions,
                                 URLPatternSet* host_permissions) {
  if (manifest_->HasKey(key)) {
    ListValue* permissions = NULL;
    if (!manifest_->GetList(key, &permissions)) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kInvalidPermissions, "");
      return false;
    }


    std::vector<std::string> host_data;
    if (!APIPermissionSet::ParseFromJSON(permissions, api_permissions,
                                         error, &host_data))
      return false;

    std::vector<APIPermission::ID> to_remove;
    FeatureProvider* permission_features =
        BaseFeatureProvider::GetPermissionFeatures();
    for (APIPermissionSet::const_iterator it = api_permissions->begin();
         it != api_permissions->end(); ++it) {
      extensions::Feature* feature =
          permission_features->GetFeature(it->name());

      CHECK(feature);

      Feature::Availability availability =
          feature->IsAvailableToManifest(
              id(),
              GetType(),
              Feature::ConvertLocation(location()),
              manifest_version());
      if (!availability.is_available()) {
        install_warnings_.push_back(InstallWarning(InstallWarning::FORMAT_TEXT,
                                                   availability.message()));
        to_remove.push_back(it->id());
        continue;
      }

      if (it->id() == APIPermission::kExperimental) {
        if (!CanSpecifyExperimentalPermission()) {
          *error = ASCIIToUTF16(errors::kExperimentalFlagRequired);
          return false;
        }
      }
    }

    for (std::vector<APIPermission::ID>::const_iterator it = to_remove.begin();
         it != to_remove.end(); ++it) {
      api_permissions->erase(*it);
    }

    const int kAllowedSchemes = CanExecuteScriptEverywhere() ?
        URLPattern::SCHEME_ALL : kValidHostPermissionSchemes;

    for (std::vector<std::string>::const_iterator it = host_data.begin();
         it != host_data.end(); ++it) {
      const std::string& permission_str = *it;

      URLPattern pattern = URLPattern(kAllowedSchemes);
      URLPattern::ParseResult parse_result = pattern.Parse(permission_str);
      if (parse_result == URLPattern::PARSE_SUCCESS) {
        if (!CanSpecifyHostPermission(pattern, *api_permissions)) {
          *error = ErrorUtils::FormatErrorMessageUTF16(
              errors::kInvalidPermissionScheme, permission_str);
          return false;
        }

        pattern.SetPath("/*");

        if (pattern.MatchesScheme(chrome::kFileScheme) &&
            !CanExecuteScriptEverywhere()) {
          wants_file_access_ = true;
          if (!(creation_flags_ & ALLOW_FILE_ACCESS)) {
            pattern.SetValidSchemes(
                pattern.valid_schemes() & ~URLPattern::SCHEME_FILE);
          }
        }

        host_permissions->AddPattern(pattern);
        continue;
      }

      install_warnings_.push_back(InstallWarning(
          InstallWarning::FORMAT_TEXT,
          base::StringPrintf(
              "Permission '%s' is unknown or URL pattern is malformed.",
              permission_str.c_str())));
    }
  }
  return true;
}
