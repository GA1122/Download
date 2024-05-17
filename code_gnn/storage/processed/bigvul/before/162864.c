void InitializeGpuPreferencesForTestingFromCommandLine(
    const base::CommandLine& command_line,
    GpuPreferences* preferences) {
  preferences->use_passthrough_cmd_decoder =
      gles2::UsePassthroughCommandDecoder(&command_line);
}
