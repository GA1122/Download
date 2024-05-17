JsArgList SyncManager::SyncInternal::GetAllNodes(
    const JsArgList& args) {
  ListValue return_args;
  ListValue* result = new ListValue();
  return_args.Append(result);

  ReadTransaction trans(FROM_HERE, GetUserShare());
  std::vector<const syncable::EntryKernel*> entry_kernels;
  trans.GetDirectory()->GetAllEntryKernels(trans.GetWrappedTrans(),
                                           &entry_kernels);

  for (std::vector<const syncable::EntryKernel*>::const_iterator it =
           entry_kernels.begin(); it != entry_kernels.end(); ++it) {
    result->Append((*it)->ToValue());
  }

  return JsArgList(&return_args);
}
