  DeleteRequest(scoped_refptr<IndexedDBDatabase> db,
                scoped_refptr<IndexedDBCallbacks> callbacks)
      : ConnectionRequest(db), callbacks_(callbacks) {}
