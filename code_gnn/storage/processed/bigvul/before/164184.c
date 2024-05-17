  void MethodWrapper(Method method) {
    SetUpTest();

    FlushAllTasks();

    base::SequencedTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::BindOnce(&AppCacheStorageImplTest::RunMethod<Method>,
                                  base::Unretained(this), method));
  }
