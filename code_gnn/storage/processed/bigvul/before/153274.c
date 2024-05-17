void DesktopWindowTreeHostX11::RestartDelayedResizeTask() {
  delayed_resize_task_.Reset(base::BindOnce(
      &DesktopWindowTreeHostX11::DelayedResize,
      close_widget_factory_.GetWeakPtr(), bounds_in_pixels_.size()));
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, delayed_resize_task_.callback());
}
