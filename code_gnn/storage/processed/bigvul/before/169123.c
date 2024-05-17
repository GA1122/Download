HTMLTreeBuilderSimulator::HTMLTreeBuilderSimulator(
    const HTMLParserOptions& options)
    : options_(options), in_select_insertion_mode_(false) {
  namespace_stack_.push_back(HTML);
}
