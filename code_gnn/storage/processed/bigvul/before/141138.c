bool Document::IsSlotAssignmentOrLegacyDistributionDirty() const {
  if (ChildNeedsDistributionRecalc())
    return true;
  if (slot_assignment_engine_ &&
      slot_assignment_engine_->HasPendingSlotAssignmentRecalc()) {
    return true;
  }
  return false;
}
