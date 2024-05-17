 virtual void SetUp() {
 AlarmTestHarness::SetUp();
      cb_counter = 0;

      semaphore = semaphore_new(0);
 }
