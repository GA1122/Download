  void RunUntilIdle() {
    loop_.RunUntilIdle();
    BrowserThread::GetBlockingPool()->FlushForTesting();
    loop_.RunUntilIdle();
  }
