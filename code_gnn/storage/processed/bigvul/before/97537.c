ProfileSyncComponentsFactoryImpl::ProfileSyncComponentsFactoryImpl(
    Profile* profile, CommandLine* command_line)
    : profile_(profile),
      command_line_(command_line),
      extension_system_(
          extensions::ExtensionSystemFactory::GetForProfile(profile)),
      web_data_service_(WebDataService::FromBrowserContext(profile_)) {
}
