bool Document::IsSlotAssignmentOrLegacyDistributionDirty() {
  if (ChildNeedsDistributionRecalc())
    return true;
  if (GetSlotAssignmentEngine().HasPendingSlotAssignmentRecalc()) {
    return true;
  }
  return false;
}
