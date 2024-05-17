SlotAssignmentEngine& Document::GetSlotAssignmentEngine() {
  if (!slot_assignment_engine_)
    slot_assignment_engine_ = SlotAssignmentEngine::Create();
  return *slot_assignment_engine_;
}
