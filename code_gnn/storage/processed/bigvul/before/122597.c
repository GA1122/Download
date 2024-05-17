void ExtensionFunctionDispatcher::GetAllFunctionNames(
    std::vector<std::string>* names) {
  ExtensionFunctionRegistry::GetInstance()->GetAllNames(names);
}
