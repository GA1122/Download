void DevToolsWindow::SetLoadCompletedCallback(const base::Closure& closure) {
  if (life_stage_ == kLoadCompleted || life_stage_ == kClosing) {
    if (!closure.is_null())
      closure.Run();
    return;
  }
  load_completed_callback_ = closure;
}
