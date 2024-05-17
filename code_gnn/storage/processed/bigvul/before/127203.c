DesktopSessionWin::DesktopSessionWin(
    scoped_refptr<base::SingleThreadTaskRunner> main_task_runner,
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    DaemonProcess* daemon_process,
    int id,
    WtsConsoleMonitor* monitor)
    : DesktopSession(daemon_process, id),
      main_task_runner_(main_task_runner),
      io_task_runner_(io_task_runner),
      monitor_(monitor) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  monitor_->AddWtsConsoleObserver(this);
}
