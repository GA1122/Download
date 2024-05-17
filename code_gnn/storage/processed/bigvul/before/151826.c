void BackgroundLoaderOfflinerTest::SetUp() {
  base::CommandLine* cl = base::CommandLine::ForCurrentProcess();
  cl->AppendSwitch(kShortSnapshotDelayForTest);

  std::unique_ptr<TestLoadTerminationListener> listener =
      std::make_unique<TestLoadTerminationListener>();
  load_termination_listener_ = listener.get();
  model_ = new MockOfflinePageModel();
  policy_.reset(new OfflinerPolicy());
  offliner_.reset(new TestBackgroundLoaderOffliner(
      profile(), policy_.get(), model_, std::move(listener)));
}
