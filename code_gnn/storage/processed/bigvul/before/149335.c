IndexedDBConnection::IndexedDBConnection(
    int child_process_id,
    scoped_refptr<IndexedDBDatabase> database,
    scoped_refptr<IndexedDBDatabaseCallbacks> callbacks)
    : id_(next_id++),
      child_process_id_(child_process_id),
      database_(database),
      callbacks_(callbacks),
      weak_factory_(this) {}
