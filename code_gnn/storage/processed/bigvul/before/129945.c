std::string FormatSampleForDiagnosticOutput(
    const Sample& sample,
    const std::vector<Module>& modules) {
  std::string output;
  for (const Frame& frame: sample) {
    output += StringPrintf(
        "0x%p %s\n", reinterpret_cast<const void*>(frame.instruction_pointer),
        modules[frame.module_index].filename.AsUTF8Unsafe().c_str());
  }
  return output;
}
