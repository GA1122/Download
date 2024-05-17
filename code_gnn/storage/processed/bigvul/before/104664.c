  void LoadAndExpectError(const std::string& name,
                          const std::string& expected_error) {
    std::string error;
    scoped_refptr<Extension> extension(LoadExtension(name, &error));
    VerifyExpectedError(extension.get(), name, error, expected_error);
  }
