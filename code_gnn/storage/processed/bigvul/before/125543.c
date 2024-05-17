void InitDirectoryService(GDataDirectoryService* directory_service) {
  int sequence_id = 1;
  GDataDirectory* dir1 = AddDirectory(directory_service->root(),
      directory_service, sequence_id++);
  GDataDirectory* dir2 = AddDirectory(directory_service->root(),
      directory_service, sequence_id++);
  GDataDirectory* dir3 = AddDirectory(dir1, directory_service, sequence_id++);

  AddFile(dir1, directory_service, sequence_id++);
  AddFile(dir1, directory_service, sequence_id++);

  AddFile(dir2, directory_service, sequence_id++);
  AddFile(dir2, directory_service, sequence_id++);
  AddFile(dir2, directory_service, sequence_id++);

  AddFile(dir3, directory_service, sequence_id++);
  AddFile(dir3, directory_service, sequence_id++);
}
