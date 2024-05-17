void MigrateTaskbarPinsCallback() {
  base::FilePath chrome_exe;
  if (!base::PathService::Get(base::FILE_EXE, &chrome_exe))
    return;

  base::FilePath pins_path;
  if (!base::PathService::Get(base::DIR_TASKBAR_PINS, &pins_path)) {
    NOTREACHED();
    return;
  }

   win::MigrateShortcutsInPathInternal(chrome_exe, pins_path);
 }
