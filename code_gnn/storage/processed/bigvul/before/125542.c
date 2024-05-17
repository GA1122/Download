GDataFile* FindFile(GDataDirectoryService* directory_service,
                    const char* path) {
  return directory_service->FindEntryByPathSync(FilePath(path))->AsGDataFile();
}