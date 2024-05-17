  ~SecurityState() {
    storage::IsolatedContext* isolated_context =
        storage::IsolatedContext::GetInstance();
    for (FileSystemMap::iterator iter = filesystem_permissions_.begin();
         iter != filesystem_permissions_.end();
         ++iter) {
      isolated_context->RemoveReference(iter->first);
    }
    UMA_HISTOGRAM_COUNTS_1M(
        "ChildProcessSecurityPolicy.PerChildFilePermissions",
        file_permissions_.size());
  }
