  void LoadTokenDatabase() {
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    base::FilePath path = temp_dir_.GetPath().AppendASCII("TestWebDB");
    scoped_refptr<WebDatabaseService> web_database =
        new WebDatabaseService(path, base::ThreadTaskRunnerHandle::Get(),
                               base::ThreadTaskRunnerHandle::Get());
    web_database->AddTable(std::make_unique<TokenServiceTable>());
    web_database->LoadDatabase();
    token_web_data_ =
        new TokenWebData(web_database, base::ThreadTaskRunnerHandle::Get(),
                         base::ThreadTaskRunnerHandle::Get(),
                         WebDataServiceBase::ProfileErrorCallback());
    token_web_data_->Init();
  }
