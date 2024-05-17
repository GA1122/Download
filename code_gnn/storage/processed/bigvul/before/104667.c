  scoped_refptr<Extension> LoadAndExpectSuccess(const std::string& name) {
    std::string error;
    scoped_refptr<Extension> extension = LoadExtension(name, &error);
    EXPECT_TRUE(extension) << name;
    EXPECT_EQ("", error) << name;
    return extension;
  }
