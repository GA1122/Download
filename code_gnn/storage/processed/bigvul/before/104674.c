  DictionaryValue* LoadManifestFile(const std::string& filename,
                                    std::string* error) {
    FilePath path;
    PathService::Get(chrome::DIR_TEST_DATA, &path);
    path = path.AppendASCII("extensions")
        .AppendASCII("manifest_tests")
        .AppendASCII(filename.c_str());
    EXPECT_TRUE(file_util::PathExists(path));

    JSONFileValueSerializer serializer(path);
    return static_cast<DictionaryValue*>(serializer.Deserialize(NULL, error));
  }
