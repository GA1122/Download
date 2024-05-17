void DevToolsHttpHandler::Send500(int connection_id,
                                  const std::string& message) {
  if (!thread_)
    return;
  thread_->task_runner()->PostTask(
      FROM_HERE, base::BindOnce(&ServerWrapper::Send500,
                                base::Unretained(server_wrapper_.get()),
                                connection_id, message));
}
