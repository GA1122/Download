void DaemonProcess::Initialize() {
  DCHECK(caller_task_runner()->BelongsToCurrentThread());

  FilePath default_config_dir = remoting::GetConfigDir();
  FilePath config_path = default_config_dir.Append(kDefaultHostConfigFile);
  const CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(kHostConfigSwitchName)) {
    config_path = command_line->GetSwitchValuePath(kHostConfigSwitchName);
  }

  config_watcher_.reset(new ConfigFileWatcher(caller_task_runner(),
                                              io_task_runner(),
                                              this));
  config_watcher_->Watch(config_path);

  LaunchNetworkProcess();
}
