void InitializeResourceBundle() {
  base::FilePath pak_file;
  bool result = base::PathService::Get(base::DIR_ASSETS, &pak_file);
  DCHECK(result);
  pak_file = pak_file.Append(FILE_PATH_LITERAL("webrunner.pak"));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(pak_file);
}
