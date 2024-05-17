WebEditingCommandType WebEditingCommandTypeFromCommandName(
    const String& command_name) {
  const CommandNameEntry* result = std::lower_bound(
      std::begin(kCommandNameEntries), std::end(kCommandNameEntries),
      command_name, [](const CommandNameEntry& entry, const String& needle) {
        return CodePointCompareIgnoringASCIICase(needle, entry.name) > 0;
      });
  if (result != std::end(kCommandNameEntries) &&
      CodePointCompareIgnoringASCIICase(command_name, result->name) == 0)
    return result->type;
  return WebEditingCommandType::kInvalid;
}
