void IndexedDBDispatcher::OnSuccessCursorContinue(
    const IndexedDBMsg_CallbacksSuccessCursorContinue_Params& p) {
  DCHECK_EQ(p.thread_id, CurrentWorkerId());
  int32 response_id = p.response_id;
  int32 cursor_id = p.cursor_id;
  const IndexedDBKey& key = p.key;
  const IndexedDBKey& primary_key = p.primary_key;
  const content::SerializedScriptValue& value = p.serialized_value;

  RendererWebIDBCursorImpl* cursor = cursors_[cursor_id];
  DCHECK(cursor);
  cursor->SetKeyAndValue(key, primary_key, value);

  WebIDBCallbacks* callbacks = pending_callbacks_.Lookup(response_id);
  if (!callbacks)
    return;
  callbacks->onSuccessWithContinuation();

  pending_callbacks_.Remove(response_id);
}
