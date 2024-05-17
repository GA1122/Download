void MasterPreferences::InitializeFromCommandLine(
    const base::CommandLine& cmd_line) {
#if defined(OS_WIN)
  if (cmd_line.HasSwitch(installer::switches::kInstallerData)) {
    base::FilePath prefs_path(cmd_line.GetSwitchValuePath(
        installer::switches::kInstallerData));
    InitializeFromFilePath(prefs_path);
  } else {
    master_dictionary_.reset(new base::DictionaryValue());
  }

  DCHECK(master_dictionary_.get());

  static const struct CmdLineSwitchToDistributionSwitch {
    const char* cmd_line_switch;
    const char* distribution_switch;
  } translate_switches[] = {
    { installer::switches::kAutoLaunchChrome,
      installer::master_preferences::kAutoLaunchChrome },
    { installer::switches::kChrome,
      installer::master_preferences::kChrome },
    { installer::switches::kDisableLogging,
      installer::master_preferences::kDisableLogging },
    { installer::switches::kMsi,
      installer::master_preferences::kMsi },
    { installer::switches::kMultiInstall,
      installer::master_preferences::kMultiInstall },
    { installer::switches::kDoNotRegisterForUpdateLaunch,
      installer::master_preferences::kDoNotRegisterForUpdateLaunch },
    { installer::switches::kDoNotLaunchChrome,
      installer::master_preferences::kDoNotLaunchChrome },
    { installer::switches::kMakeChromeDefault,
      installer::master_preferences::kMakeChromeDefault },
    { installer::switches::kSystemLevel,
      installer::master_preferences::kSystemLevel },
    { installer::switches::kVerboseLogging,
      installer::master_preferences::kVerboseLogging },
  };

  std::string name(installer::master_preferences::kDistroDict);
  for (int i = 0; i < arraysize(translate_switches); ++i) {
    if (cmd_line.HasSwitch(translate_switches[i].cmd_line_switch)) {
      name.assign(installer::master_preferences::kDistroDict);
      name.append(".").append(translate_switches[i].distribution_switch);
      master_dictionary_->SetBoolean(name, true);
    }
  }

  std::wstring str_value(cmd_line.GetSwitchValueNative(
      installer::switches::kLogFile));
  if (!str_value.empty()) {
    name.assign(installer::master_preferences::kDistroDict);
    name.append(".").append(installer::master_preferences::kLogFile);
    master_dictionary_->SetString(name, str_value);
  }

  scoped_ptr<base::Environment> env(base::Environment::Create());
  if (env != NULL) {
    std::string is_machine_var;
    env->GetVar(env_vars::kGoogleUpdateIsMachineEnvVar, &is_machine_var);
    if (!is_machine_var.empty() && is_machine_var[0] == '1') {
      VLOG(1) << "Taking system-level from environment.";
      name.assign(installer::master_preferences::kDistroDict);
      name.append(".").append(installer::master_preferences::kSystemLevel);
      master_dictionary_->SetBoolean(name, true);
    }
  }

  master_dictionary_->GetDictionary(installer::master_preferences::kDistroDict,
                                    &distribution_);

  InitializeProductFlags();
#endif
}
