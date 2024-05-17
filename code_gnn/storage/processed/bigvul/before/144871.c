  void RunTasksForPeriod(double delay_ms) {
    TimeAdvance();
    scheduler::GetSingleThreadTaskRunnerForTesting()->PostDelayedTask(
        FROM_HERE,
        WTF::Bind(
            &ScrollbarsTestWithVirtualTimer::StopVirtualTimeAndExitRunLoop,
            WTF::Unretained(this)),
        TimeDelta::FromMillisecondsD(delay_ms));
    testing::EnterRunLoop();
  }
