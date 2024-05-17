  void TestFinished() {
    DCHECK(io_runner->BelongsToCurrentThread());
    base::SequencedTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::BindOnce(&AppCacheStorageImplTest::TestFinishedUnwound,
                                  base::Unretained(this)));
  }
