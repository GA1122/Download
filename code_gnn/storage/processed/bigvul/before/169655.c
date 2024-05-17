  void AssertForegroundAndRepost(const base::Process& renderer_process) {
    ASSERT_FALSE(renderer_process.IsProcessBackgrounded());
    base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
        FROM_HERE,
        base::BindOnce(&AssertForegroundHelper::AssertForegroundAndRepost,
                       weak_ptr_factory_.GetWeakPtr(),
                       base::ConstRef(renderer_process)),
        base::TimeDelta::FromMilliseconds(1));
  }
