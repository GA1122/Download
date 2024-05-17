scoped_refptr<Extension> LoadManifest(const std::string& dir,
                                      const std::string& test_file,
                                      int extra_flags) {
  FilePath path;
  PathService::Get(chrome::DIR_TEST_DATA, &path);
  path = path.AppendASCII("extensions")
             .AppendASCII(dir)
             .AppendASCII(test_file);

  JSONFileValueSerializer serializer(path);
  std::string error;
  scoped_ptr<Value> result(serializer.Deserialize(NULL, &error));
  if (!result.get()) {
    EXPECT_EQ("", error);
    return NULL;
  }

  scoped_refptr<Extension> extension = Extension::Create(
      path.DirName(), Extension::INVALID,
      *static_cast<DictionaryValue*>(result.get()), extra_flags, &error);
  EXPECT_TRUE(extension) << error;
  return extension;
}
