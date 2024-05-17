base::FilePath ChromeContentBrowserClient::GetHyphenDictionaryDirectory() {
  base::FilePath directory;
  PathService::Get(chrome::DIR_APP_DICTIONARIES, &directory);
  return directory.Append(FILE_PATH_LITERAL("Hyphen"));
}
