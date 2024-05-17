  void LoadAllPdfsTest(const std::string& dir_name, int k) {
    base::FilePath test_data_dir;
    ASSERT_TRUE(PathService::Get(chrome::DIR_TEST_DATA, &test_data_dir));
    base::FileEnumerator file_enumerator(test_data_dir.AppendASCII(dir_name),
                                         false, base::FileEnumerator::FILES,
                                         FILE_PATH_LITERAL("*.pdf"));

    size_t count = 0;
    for (base::FilePath file_path = file_enumerator.Next(); !file_path.empty();
         file_path = file_enumerator.Next()) {
      std::string filename = file_path.BaseName().MaybeAsASCII();
      ASSERT_FALSE(filename.empty());

      std::string pdf_file = dir_name + "/" + filename;
      if (static_cast<int>(base::Hash(filename) % kNumberLoadTestParts) == k) {
        LOG(INFO) << "Loading: " << pdf_file;
        bool success = LoadPdf(embedded_test_server()->GetURL("/" + pdf_file));
        EXPECT_EQ(!PdfIsExpectedToFailLoad(pdf_file), success);
      }
      ++count;
    }
     ASSERT_GE(count, 1u);
   }
