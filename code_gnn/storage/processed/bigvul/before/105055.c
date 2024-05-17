Profile* LiveSyncTest::MakeProfile(const FilePath::StringType name) {
  FilePath path;
  PathService::Get(chrome::DIR_USER_DATA, &path);
  path = path.Append(name);

  if (!file_util::PathExists(path))
    CHECK(file_util::CreateDirectory(path));

  return Profile::CreateProfile(path);
}
