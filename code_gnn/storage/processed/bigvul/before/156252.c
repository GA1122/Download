const char* RendererProcessTypeToString(RendererProcessType process_type) {
  switch (process_type) {
    case RendererProcessType::kRenderer:
      return "normal";
    case RendererProcessType::kExtensionRenderer:
      return "extension";
  }
  NOTREACHED();
  return "";   
}
