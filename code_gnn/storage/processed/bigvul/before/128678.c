bool ExtensionsDisabled(const base::CommandLine& command_line) {
  return command_line.HasSwitch(switches::kDisableExtensions) ||
         command_line.HasSwitch(switches::kDisableExtensionsExcept);
}
