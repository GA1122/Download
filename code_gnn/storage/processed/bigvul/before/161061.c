DevToolsHttpHandler::DevToolsHttpHandler(
    DevToolsManagerDelegate* delegate,
    std::unique_ptr<DevToolsSocketFactory> socket_factory,
    const base::FilePath& output_directory,
    const base::FilePath& debug_frontend_dir)
    : delegate_(delegate), weak_factory_(this) {
  browser_guid_ = delegate_->IsBrowserTargetDiscoverable()
                      ? kBrowserUrlPrefix
                      : base::StringPrintf("%s/%s", kBrowserUrlPrefix,
                                           base::GenerateGUID().c_str());
  std::unique_ptr<base::Thread> thread(
      new base::Thread(kDevToolsHandlerThreadName));
  base::Thread::Options options;
  options.message_loop_type = base::MessageLoop::TYPE_IO;
  if (thread->StartWithOptions(options)) {
    base::TaskRunner* task_runner = thread->task_runner().get();
    task_runner->PostTask(
        FROM_HERE,
        base::BindOnce(&StartServerOnHandlerThread, weak_factory_.GetWeakPtr(),
                       std::move(thread), std::move(socket_factory),
                       output_directory, debug_frontend_dir, browser_guid_,
                       delegate_->HasBundledFrontendResources()));
  }
}
