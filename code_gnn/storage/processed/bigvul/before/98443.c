  void LoadAndExpectError(const std::string& name,
                          const std::string& expected_error) {
    std::string error;
    scoped_ptr<Extension> extension(LoadExtension(name, &error));
    EXPECT_FALSE(extension.get()) <<
        "Expected failure loading extension '" << name <<
        "', but didn't get one.";
    EXPECT_TRUE(MatchPatternASCII(error, expected_error)) << name <<
        " expected '" << expected_error << "' but got '" << error << "'";
  }
