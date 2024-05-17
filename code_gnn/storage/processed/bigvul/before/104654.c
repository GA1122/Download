UnloadedExtensionInfo::UnloadedExtensionInfo(
    const Extension* extension,
    Reason reason)
  : reason(reason),
    already_disabled(false),
    extension(extension) {}
