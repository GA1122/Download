  void DidGetGlobalUsage(StorageType type, int64, int64) {
    AddEntries(type, manager()->GetUsageTracker(type));
  }
