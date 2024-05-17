  SessionStorageHolder()
      : session_storage_namespaces_awaiting_close_(
            new std::map<int, SessionStorageNamespaceMap>) {
  }
