void TerminateOnUI(std::unique_ptr<base::Thread> thread,
                   std::unique_ptr<ServerWrapper> server_wrapper,
                   std::unique_ptr<DevToolsSocketFactory> socket_factory) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (server_wrapper)
    thread->task_runner()->DeleteSoon(FROM_HERE, std::move(server_wrapper));
  if (socket_factory)
    thread->task_runner()->DeleteSoon(FROM_HERE, std::move(socket_factory));
  if (thread) {
    base::PostTaskWithTraits(
        FROM_HERE, {base::MayBlock(), base::TaskPriority::BACKGROUND},
        BindOnce([](std::unique_ptr<base::Thread>) {}, std::move(thread)));
  }
}
