bool InputType::GetAllowedValueStep(Decimal* step) const {
  StepRange step_range(CreateStepRange(kRejectAny));
  *step = step_range.Step();
  return step_range.HasStep();
}
