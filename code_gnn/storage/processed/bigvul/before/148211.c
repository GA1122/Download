CursorImpl::CursorImpl(std::unique_ptr<IndexedDBCursor> cursor,
                       const url::Origin& origin,
                       IndexedDBDispatcherHost* dispatcher_host,
                       scoped_refptr<base::SequencedTaskRunner> idb_runner)
    : helper_(new IDBThreadHelper(std::move(cursor))),
      dispatcher_host_(dispatcher_host),
      origin_(origin),
      idb_runner_(std::move(idb_runner)) {}
