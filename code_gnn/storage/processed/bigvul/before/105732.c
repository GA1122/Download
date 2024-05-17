IBusInputContext* GetInputContext(
    const std::string& input_context_path, IBusBus* ibus) {
  GDBusConnection* connection = ibus_bus_get_connection(ibus);
  if (!connection) {
    LOG(ERROR) << "IBusConnection is null";
    return NULL;
  }
  IBusInputContext* context = ibus_input_context_get_input_context(
      input_context_path.c_str(), connection);
  if (!context) {
    LOG(ERROR) << "IBusInputContext is null: " << input_context_path;
  }
  return context;
}
