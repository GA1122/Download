void Automation::Init(const CommandLine& options,
                      Error** error) {
  FilePath browser_exe;
  if (!GetDefaultChromeExe(&browser_exe)) {
    *error = new Error(kUnknownError, "Could not find default Chrome binary");
    return;
  }

  InitWithBrowserPath(browser_exe, options, error);
}
