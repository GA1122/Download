  void VerifyExpectedError(Extension* extension,
                           const std::string& name,
                           const std::string& error,
                           const std::string& expected_error) {
    EXPECT_FALSE(extension) <<
        "Expected failure loading extension '" << name <<
        "', but didn't get one.";
    EXPECT_TRUE(MatchPattern(error, expected_error)) << name <<
        " expected '" << expected_error << "' but got '" << error << "'";
  }
