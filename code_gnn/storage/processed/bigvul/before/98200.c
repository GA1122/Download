void WebSocketExperimentRunner::Start() {
  DCHECK(!runner.get());

  scoped_refptr<FieldTrial> trial = new FieldTrial("WebSocketExperiment", 1000);
  trial->AppendGroup("active", 5);   

  bool run_experiment = (trial->group() != FieldTrial::kNotParticipating);
#ifndef NDEBUG
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  std::string experiment_host = command_line.GetSwitchValueASCII(
      switches::kWebSocketLiveExperimentHost);
  if (!experiment_host.empty())
    run_experiment = true;
#endif
  if (!run_experiment)
    return;

  runner = new WebSocketExperimentRunner;
  runner->Run();
}
