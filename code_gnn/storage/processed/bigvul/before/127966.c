void BrowserViewRenderer::TrimMemory(const int level, const bool visible) {
  DCHECK(ui_task_runner_->BelongsToCurrentThread());
  enum {
    TRIM_MEMORY_RUNNING_LOW = 10,
    TRIM_MEMORY_UI_HIDDEN = 20,
    TRIM_MEMORY_BACKGROUND = 40,
    TRIM_MEMORY_MODERATE = 60,
  };

  if (level < TRIM_MEMORY_RUNNING_LOW || level == TRIM_MEMORY_UI_HIDDEN)
    return;

  if (level < TRIM_MEMORY_BACKGROUND && visible)
    return;

  if (!compositor_ || !hardware_enabled_)
    return;

  TRACE_EVENT0("android_webview", "BrowserViewRenderer::TrimMemory");

  if (level >= TRIM_MEMORY_MODERATE) {
    if (offscreen_pre_raster_)
      shared_renderer_state_.DeleteHardwareRendererOnUI();
    else
      shared_renderer_state_.ReleaseHardwareDrawIfNeededOnUI();
    return;
  }

  if (!offscreen_pre_raster_)
    compositor_->SetMemoryPolicy(0u);
}
