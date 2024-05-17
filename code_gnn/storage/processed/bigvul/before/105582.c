bool GetDefaultChromeExe(FilePath* browser_exe) {
  std::vector<FilePath> locations;
  FilePath module_dir;
  if (PathService::Get(base::DIR_MODULE, &module_dir))
    locations.push_back(module_dir);

#if defined(OS_WIN)
  const wchar_t kSubKey[] =
      L"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe";
  base::win::RegKey key(HKEY_CURRENT_USER, kSubKey, KEY_READ);
  std::wstring path;
  if (key.ReadValue(L"path", &path) == ERROR_SUCCESS)
    locations.push_back(FilePath(path));
  base::win::RegKey sys_key(HKEY_LOCAL_MACHINE, kSubKey, KEY_READ);
  if (sys_key.ReadValue(L"path", &path) == ERROR_SUCCESS)
    locations.push_back(FilePath(path));

  FilePath app_from_google(L"Google\\Chrome\\Application");
  scoped_ptr<base::Environment> env(base::Environment::Create());
  std::string home_dir;
  if (env->GetVar("userprofile", &home_dir)) {
    FilePath default_location(UTF8ToWide(home_dir));
    if (base::win::GetVersion() < base::win::VERSION_VISTA) {
      default_location = default_location.Append(
          L"Local Settings\\Application Data");
    } else {
      default_location = default_location.Append(L"AppData\\Local");
    }
    locations.push_back(default_location.Append(app_from_google));
  }

  std::string program_dir;
  if (env->GetVar("ProgramFiles", &program_dir)) {
    locations.push_back(FilePath(UTF8ToWide(program_dir))
        .Append(app_from_google));
  }
  if (env->GetVar("ProgramFiles(x86)", &program_dir)) {
    locations.push_back(FilePath(UTF8ToWide(program_dir))
        .Append(app_from_google));
  }
#elif defined(OS_MACOSX)
  locations.push_back(FilePath("/Applications"));
#elif defined(OS_LINUX)
  FilePath chrome_sym_link("/usr/bin/google-chrome");
  if (file_util::PathExists(chrome_sym_link)) {
    FilePath chrome;
    if (file_util::ReadSymbolicLink(chrome_sym_link, &chrome)) {
      locations.push_back(chrome.DirName());
    }
  }
#endif

  FilePath current_dir;
  if (file_util::GetCurrentDirectory(&current_dir))
    locations.push_back(current_dir);

  for (size_t i = 0; i < locations.size(); ++i) {
    FilePath path = locations[i].Append(chrome::kBrowserProcessExecutablePath);
    if (file_util::PathExists(path)) {
      *browser_exe = path;
      return true;
    }
  }
  return false;
}
