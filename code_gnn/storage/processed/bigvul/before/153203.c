void DesktopWindowTreeHostX11::Close() {
  content_window()->Hide();

  delayed_resize_task_.Cancel();

  if (!close_widget_factory_.HasWeakPtrs()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::BindOnce(&DesktopWindowTreeHostX11::CloseNow,
                                  close_widget_factory_.GetWeakPtr()));
  }
}
