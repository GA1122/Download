 Profile* SyncTest::MakeProfile(const FilePath::StringType name) {
  FilePath path;
  PathService::Get(chrome::DIR_USER_DATA, &path);
  path = path.Append(name);

  if (!file_util::PathExists(path))
    CHECK(file_util::CreateDirectory(path));

  Profile* profile =
      Profile::CreateProfile(path, NULL, Profile::CREATE_MODE_SYNCHRONOUS);
  g_browser_process->profile_manager()->RegisterTestingProfile(profile, true);
  return profile;
}
