void WallpaperManagerBase::SetCommandLineForTesting(
    base::CommandLine* command_line) {
  command_line_for_testing_ = command_line;
  SetDefaultWallpaperPathsFromCommandLine(command_line);
}
