void RegisterComponentsForUpdate() {
  component_updater::ComponentUpdateService* cus =
      g_browser_process->component_updater();

#if !defined(OS_CHROMEOS) && !defined(OS_ANDROID)
  RegisterRecoveryComponent(cus, g_browser_process->local_state());
  RegisterPepperFlashComponent(cus);
  RegisterSwiftShaderComponent(cus);
  RegisterWidevineCdmComponent(cus);
#endif   

#if !defined(DISABLE_NACL) && !defined(OS_ANDROID)
#if defined(OS_CHROMEOS)
  if (!base::SysInfo::IsRunningOnChromeOS())
#endif   
    g_browser_process->pnacl_component_installer()->RegisterPnaclComponent(cus);
#endif   

  RegisterCldComponent(cus);

  component_updater::SupervisedUserWhitelistInstaller* whitelist_installer =
      g_browser_process->supervised_user_whitelist_installer();
  whitelist_installer->RegisterComponents();

  base::FilePath path;
  if (PathService::Get(chrome::DIR_USER_DATA, &path)) {
#if defined(OS_ANDROID)
    g_browser_process->crl_set_fetcher()->DeleteFromDisk(path);
#elif !defined(OS_CHROMEOS)
    g_browser_process->crl_set_fetcher()->StartInitialLoad(cus, path);
    RegisterEVWhitelistComponent(cus, path);
#endif   
  }

#if defined(OS_WIN)
#if defined(GOOGLE_CHROME_BUILD)
  RegisterSwReporterComponent(cus);
#endif   
  RegisterCAPSComponent(cus);
#endif   
}
