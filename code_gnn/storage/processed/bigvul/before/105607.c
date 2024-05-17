WebDriverCommand::WebDriverCommand(
    const std::vector<std::string>& path_segments,
    const DictionaryValue* const parameters)
    : Command(path_segments, parameters), session_(NULL) {
}
