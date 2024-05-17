ProfileSyncComponentsFactoryImpl::ProfileSyncComponentsFactoryImpl(
    Profile* profile, CommandLine* command_line)
    : profile_(profile),
      command_line_(command_line),
      extension_system_(
          ExtensionSystemFactory::GetForProfile(profile)),
      web_data_service_(WebDataServiceFactory::GetForProfile(
          profile_, Profile::IMPLICIT_ACCESS)) {
}
