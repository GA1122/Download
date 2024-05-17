ResourcePrefetchPredictorTest::ResourcePrefetchPredictorTest()
    : profile_(std::make_unique<TestingProfile>()),
      db_task_runner_(base::MakeRefCounted<base::TestSimpleTaskRunner>()),
      mock_tables_(
          base::MakeRefCounted<StrictMock<MockResourcePrefetchPredictorTables>>(
              db_task_runner_)) {}
