void PlatformSensorWin::OnSensorError() {
  task_runner_->PostTask(FROM_HERE,
                         base::Bind(&PlatformSensorWin::NotifySensorError,
                                    weak_factory_.GetWeakPtr()));
}
