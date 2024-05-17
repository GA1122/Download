void WebSocketExperimentRunner::InitConfig() {
  config_.initial_delay_ms = 5 * 60 * 1000;   
  config_.next_delay_ms = 12 * 60 * 60 * 1000;   

  std::string experiment_host = kExperimentHost;
#ifndef NDEBUG
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  std::string experiment_host_override = command_line.GetSwitchValueASCII(
      switches::kWebSocketLiveExperimentHost);
  if (!experiment_host_override.empty()) {
    experiment_host = experiment_host_override;
    config_.initial_delay_ms = 5 * 1000;   
  }
#endif

  WebSocketExperimentTask::Config* config;
  WebSocketExperimentTask::Config task_config;

  task_config.protocol_version = net::WebSocket::DEFAULT_VERSION;
  config = &config_.ws_config[STATE_RUN_WS - STATE_RUN_WS];
  *config = task_config;
  config->url =
      GURL(StringPrintf("ws://%s/live_exp", experiment_host.c_str()));
  config->ws_location =
      StringPrintf("ws://%s/live_exp", experiment_host.c_str());
  config->http_url =
      GURL(StringPrintf("http://%s/", experiment_host.c_str()));

  config = &config_.ws_config[STATE_RUN_WSS - STATE_RUN_WS];
  *config = task_config;
  config->url =
      GURL(StringPrintf("wss://%s/live_exp", experiment_host.c_str()));
  config->ws_location =
      StringPrintf("wss://%s/live_exp", experiment_host.c_str());
  config->http_url =
      GURL(StringPrintf("https://%s/", experiment_host.c_str()));

  config = &config_.ws_config[STATE_RUN_WS_NODEFAULT_PORT -
                              STATE_RUN_WS];
  *config = task_config;
  config->url =
      GURL(StringPrintf("ws://%s:%d/live_exp",
                        experiment_host.c_str(), kAlternativePort));
  config->ws_location =
      StringPrintf("ws://%s:%d/live_exp",
                   experiment_host.c_str(), kAlternativePort);
  config->http_url =
      GURL(StringPrintf("http://%s:%d/",
                        experiment_host.c_str(), kAlternativePort));

  task_config.protocol_version = net::WebSocket::DRAFT75;
  config = &config_.ws_config[STATE_RUN_WS_DRAFT75 - STATE_RUN_WS];
  *config = task_config;
  config->url =
      GURL(StringPrintf("ws://%s/live_exp", experiment_host.c_str()));
  config->ws_location =
      StringPrintf("ws://%s/live_exp", experiment_host.c_str());
  config->http_url =
      GURL(StringPrintf("http://%s/", experiment_host.c_str()));

  config = &config_.ws_config[STATE_RUN_WSS_DRAFT75 - STATE_RUN_WS];
  *config = task_config;
  config->url =
      GURL(StringPrintf("wss://%s/live_exp", experiment_host.c_str()));
  config->ws_location =
      StringPrintf("wss://%s/live_exp", experiment_host.c_str());
  config->http_url =
      GURL(StringPrintf("https://%s/", experiment_host.c_str()));

  config = &config_.ws_config[STATE_RUN_WS_NODEFAULT_PORT_DRAFT75 -
                              STATE_RUN_WS];
  *config = task_config;
  config->url =
      GURL(StringPrintf("ws://%s:%d/live_exp",
                        experiment_host.c_str(), kAlternativePort));
  config->ws_location =
      StringPrintf("ws://%s:%d/live_exp",
                   experiment_host.c_str(), kAlternativePort);
  config->http_url =
      GURL(StringPrintf("http://%s:%d/",
                        experiment_host.c_str(), kAlternativePort));

}
