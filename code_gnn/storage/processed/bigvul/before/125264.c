static void TestVfsFilePath(bool expected_result,
                            const char* vfs_file_name,
                            const char* expected_origin_identifier = "",
                            const char* expected_database_name = "",
                            const char* expected_sqlite_suffix = "") {
  string16 origin_identifier;
  string16 database_name;
  string16 sqlite_suffix;
  EXPECT_EQ(expected_result,
            DatabaseUtil::CrackVfsFileName(ASCIIToUTF16(vfs_file_name),
                                           &origin_identifier,
                                           &database_name,
                                           &sqlite_suffix));
  EXPECT_EQ(ASCIIToUTF16(expected_origin_identifier), origin_identifier);
  EXPECT_EQ(ASCIIToUTF16(expected_database_name), database_name);
  EXPECT_EQ(ASCIIToUTF16(expected_sqlite_suffix), sqlite_suffix);
}
