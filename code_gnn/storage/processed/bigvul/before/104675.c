  scoped_refptr<Extension> LoadStrictAndExpectSuccess(const std::string& name) {
    std::string error;
    scoped_refptr<Extension> extension = LoadExtensionStrict(name, &error);
    EXPECT_TRUE(extension) << name;
    EXPECT_EQ("", error) << name;
    return extension;
  }
