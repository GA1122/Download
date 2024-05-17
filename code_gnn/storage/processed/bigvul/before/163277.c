void BrowserMainLoop::MainMessageLoopStart() {

  TRACE_EVENT0("startup", "BrowserMainLoop::MainMessageLoopStart");

  if (!base::MessageLoop::current())
    main_message_loop_.reset(new base::MessageLoopForUI);

  InitializeMainThread();
}
