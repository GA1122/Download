scoped_refptr<Extension> LoadManifest(const std::string& dir,
                                      const std::string& test_file) {
  return LoadManifest(dir, test_file, Extension::NO_FLAGS);
}
