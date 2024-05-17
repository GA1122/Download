  void AssertForegroundAndRepost(const base::Process& renderer_process,
                                 base::PortProvider* port_provider) {
    ASSERT_FALSE(renderer_process.IsProcessBackgrounded(port_provider));
    base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
        FROM_HERE,
        base::BindOnce(&AssertForegroundHelper::AssertForegroundAndRepost,
                       weak_ptr_factory_.GetWeakPtr(),
                       base::ConstRef(renderer_process), port_provider),
        base::TimeDelta::FromMilliseconds(1));
  }
