  Extension* LoadAndExpectSuccess(const std::string& name) {
    std::string error;
    Extension* extension = LoadExtension(name, &error);
    EXPECT_TRUE(extension);
    EXPECT_EQ("", error);
    return extension;
  }
