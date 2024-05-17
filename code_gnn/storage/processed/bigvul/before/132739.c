void ChromotingInstance::LogToWebapp(const std::string& message) {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  LOG(ERROR) << message;

#if !defined(OS_NACL)
  ProcessLogToUI(message);
#endif   
}
