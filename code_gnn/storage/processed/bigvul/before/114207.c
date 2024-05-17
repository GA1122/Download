GpuMessageFilter::GpuMessageFilter(int render_process_id,
                                   RenderWidgetHelper* render_widget_helper)
    : gpu_process_id_(0),
      render_process_id_(render_process_id),
      share_contexts_(false),
      render_widget_helper_(render_widget_helper) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kUIUseGPUProcess)) {
    share_contexts_ = true;
  }
}
