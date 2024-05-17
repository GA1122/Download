void DevToolsHttpHandler::Send404(int connection_id) {
  if (!thread_)
    return;
  thread_->task_runner()->PostTask(
      FROM_HERE,
      base::BindOnce(&ServerWrapper::Send404,
                     base::Unretained(server_wrapper_.get()), connection_id));
}
