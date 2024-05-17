void ShelfBackgroundAnimatorTest::SetUp() {
  task_runner_ = new base::TestMockTimeTaskRunner();
  task_runner_handle_.reset(new base::ThreadTaskRunnerHandle(task_runner_));

  animator_.reset(
      new ShelfBackgroundAnimator(SHELF_BACKGROUND_DEFAULT, nullptr, nullptr));
  animator_->AddObserver(&observer_);

  test_api_.reset(new ShelfBackgroundAnimatorTestApi(animator_.get()));
}
