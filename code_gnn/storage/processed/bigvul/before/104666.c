  void LoadAndExpectErrorStrict(const std::string& name,
                                const std::string& expected_error) {
    std::string error;
    scoped_refptr<Extension> extension(LoadExtensionStrict(name, &error));
    VerifyExpectedError(extension.get(), name, error, expected_error);
  }
