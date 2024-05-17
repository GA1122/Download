DesktopSessionWin::~DesktopSessionWin() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  launcher_.reset();
   monitor_->RemoveWtsConsoleObserver(this);
 }
