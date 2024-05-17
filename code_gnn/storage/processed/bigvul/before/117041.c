bool BaseSessionService::RestoreSetTabExtensionAppIDCommand(
    const SessionCommand& command,
    SessionID::id_type* tab_id,
    std::string* extension_app_id) {
  scoped_ptr<Pickle> pickle(command.PayloadAsPickle());
  if (!pickle.get())
    return false;

  void* iterator = NULL;
  return pickle->ReadInt(&iterator, tab_id) &&
      pickle->ReadString(&iterator, extension_app_id);
}
