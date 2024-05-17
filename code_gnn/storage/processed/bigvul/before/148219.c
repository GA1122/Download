IndexedDBTransaction::Operation BindWeakOperation(
    Functor&& functor,
    base::WeakPtr<IndexedDBCursor> weak_cursor,
    Args&&... args) {
  DCHECK(weak_cursor);
  IndexedDBCursor* cursor_ptr = weak_cursor.get();
  return base::Bind(
      &InvokeOrSucceed, std::move(weak_cursor),
      base::Bind(std::forward<Functor>(functor), base::Unretained(cursor_ptr),
                 std::forward<Args>(args)...));
}
