  scoped_refptr<Extension> LoadExtensionWithLocation(
      DictionaryValue* value,
      Extension::Location location,
      bool strict_error_checks,
      std::string* error) {
    FilePath path;
    PathService::Get(chrome::DIR_TEST_DATA, &path);
    path = path.AppendASCII("extensions").AppendASCII("manifest_tests");
    int flags = Extension::NO_FLAGS;
    if (strict_error_checks)
      flags |= Extension::STRICT_ERROR_CHECKS;
    return Extension::Create(path.DirName(), location, *value, flags, error);
  }
