void IndexedDBDispatcher::OnSuccessOpenCursor(
    const IndexedDBMsg_CallbacksSuccessIDBCursor_Params& p) {
  DCHECK_EQ(p.thread_id, CurrentWorkerId());
  int32 response_id = p.response_id;
  int32 object_id = p.cursor_id;
  const IndexedDBKey& key = p.key;
  const IndexedDBKey& primary_key = p.primary_key;
  const content::SerializedScriptValue& value = p.serialized_value;

  WebIDBCallbacks* callbacks =
      pending_callbacks_.Lookup(response_id);
  if (!callbacks)
    return;

  RendererWebIDBCursorImpl* cursor = new RendererWebIDBCursorImpl(object_id);
  cursors_[object_id] = cursor;
  cursor->SetKeyAndValue(key, primary_key, value);
  callbacks->onSuccess(cursor);

  pending_callbacks_.Remove(response_id);
}
