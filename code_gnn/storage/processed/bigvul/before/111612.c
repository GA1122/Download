  void VerifyHostedDocumentJSONFile(const GDataFile* gdata_file,
                                    const FilePath& file_path) {
    ASSERT_TRUE(gdata_file != NULL);

    std::string error;
    JSONFileValueSerializer serializer(file_path);
    scoped_ptr<Value> value(serializer.Deserialize(NULL, &error));
    ASSERT_TRUE(value.get()) << "Parse error " << file_path.value()
                             << ": " << error;
    DictionaryValue* dict_value = NULL;
    ASSERT_TRUE(value->GetAsDictionary(&dict_value));

    std::string edit_url, resource_id;
    EXPECT_TRUE(dict_value->GetString("url", &edit_url));
    EXPECT_TRUE(dict_value->GetString("resource_id", &resource_id));

    EXPECT_EQ(gdata_file->alternate_url().spec(), edit_url);
    EXPECT_EQ(gdata_file->resource_id(), resource_id);
  }
