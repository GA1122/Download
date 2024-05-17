void BrowserMainLoop::RunMainMessageLoopParts() {
  TRACE_EVENT_ASYNC_BEGIN0("toplevel", "BrowserMain:MESSAGE_LOOP", this);

  bool ran_main_loop = false;
  if (parts_)
    ran_main_loop = parts_->MainMessageLoopRun(&result_code_);

  if (!ran_main_loop)
    MainMessageLoopRun();

  TRACE_EVENT_ASYNC_END0("toplevel", "BrowserMain:MESSAGE_LOOP", this);
}
