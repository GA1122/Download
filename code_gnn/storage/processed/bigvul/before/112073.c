JsArgList SyncManager::SyncInternal::GetChildNodeIds(
    const JsArgList& args) {
  ListValue return_args;
  ListValue* child_ids = new ListValue();
  return_args.Append(child_ids);
  int64 id = GetId(args.Get(), 0);
  if (id != kInvalidId) {
    ReadTransaction trans(FROM_HERE, GetUserShare());
    syncable::Directory::ChildHandles child_handles;
    trans.GetDirectory()->GetChildHandlesByHandle(trans.GetWrappedTrans(),
                                                  id, &child_handles);
    for (syncable::Directory::ChildHandles::const_iterator it =
             child_handles.begin(); it != child_handles.end(); ++it) {
      child_ids->Append(Value::CreateStringValue(
          base::Int64ToString(*it)));
    }
  }
  return JsArgList(&return_args);
}
