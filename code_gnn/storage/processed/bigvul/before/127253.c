WtsConsoleSessionProcessDriver::~WtsConsoleSessionProcessDriver() {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());

  CHECK_EQ(stoppable_state(), Stoppable::kStopped);

  monitor_->RemoveWtsConsoleObserver(this);

   CHECK(launcher_.get() == NULL);
 }
