bool StartupBrowserCreator::ProcessLoadApps(
    const base::CommandLine& command_line,
    const base::FilePath& cur_dir,
    Profile* profile) {
  base::CommandLine::StringType path_list =
      command_line.GetSwitchValueNative(extensions::switches::kLoadApps);

  base::StringTokenizerT<base::CommandLine::StringType,
                         base::CommandLine::StringType::const_iterator>
      tokenizer(path_list, FILE_PATH_LITERAL(","));

  if (!tokenizer.GetNext())
    return false;

  base::FilePath app_absolute_dir =
      base::MakeAbsoluteFilePath(base::FilePath(tokenizer.token()));
  if (!apps::AppLoadService::Get(profile)->LoadAndLaunch(
          app_absolute_dir, command_line, cur_dir)) {
    return false;
  }

  while (tokenizer.GetNext()) {
    app_absolute_dir =
        base::MakeAbsoluteFilePath(base::FilePath(tokenizer.token()));

    if (!apps::AppLoadService::Get(profile)->Load(app_absolute_dir)) {
      return false;
    }
  }

  return true;
}
