  static bool VisiblePositionsDiffer(
      const syncable::EntryKernelMutation& mutation) {
    const syncable::EntryKernel& a = mutation.original;
    const syncable::EntryKernel& b = mutation.mutated;
    if (!ShouldMaintainPosition(
            syncable::GetModelTypeFromSpecifics(b.ref(SPECIFICS))))
      return false;
    if (a.ref(syncable::NEXT_ID) != b.ref(syncable::NEXT_ID))
      return true;
    if (a.ref(syncable::PARENT_ID) != b.ref(syncable::PARENT_ID))
      return true;
    return false;
  }
