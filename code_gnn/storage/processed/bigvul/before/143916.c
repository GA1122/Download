bool WebRunnerMainDelegate::BasicStartupComplete(int* exit_code) {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  InitLoggingFromCommandLine(*command_line);
  content_client_ = std::make_unique<WebRunnerContentClient>();
  SetContentClient(content_client_.get());
  return false;
}
