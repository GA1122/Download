JsArgList SyncManager::SyncInternal::FindNodesContainingString(
    const JsArgList& args) {
  std::string query;
  ListValue return_args;
  if (!args.Get().GetString(0, &query)) {
    return_args.Append(new ListValue());
    return JsArgList(&return_args);
  }

  std::string lowercase_query = query;
  StringToLowerASCII(&lowercase_query);

  ListValue* result = new ListValue();
  return_args.Append(result);

  ReadTransaction trans(FROM_HERE, GetUserShare());
  std::vector<const syncable::EntryKernel*> entry_kernels;
  trans.GetLookup()->GetAllEntryKernels(trans.GetWrappedTrans(),
                                        &entry_kernels);

  for (std::vector<const syncable::EntryKernel*>::const_iterator it =
           entry_kernels.begin(); it != entry_kernels.end(); ++it) {
    if ((*it)->ContainsString(lowercase_query)) {
      result->Append(new StringValue(base::Int64ToString(
          (*it)->ref(syncable::META_HANDLE))));
    }
  }

  return JsArgList(&return_args);
}
