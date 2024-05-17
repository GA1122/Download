  RunLoopTestEnvironment(RunLoopTestType type) {
    switch (type) {
      case RunLoopTestType::kRealEnvironment:
        task_environment_ = base::MakeUnique<test::ScopedTaskEnvironment>();
        break;
      case RunLoopTestType::kTestDelegate:
        test_delegate_ = base::MakeUnique<TestDelegate>();
        test_delegate_->BindToCurrentThread();
        break;
    }
  }
