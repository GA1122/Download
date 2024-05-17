GDataFileSystem* GetGDataFileSystem(Profile* profile) {
  GDataSystemService* system_service =
      GDataSystemServiceFactory::GetForProfile(profile);
  return system_service ? system_service->file_system() : NULL;
}
