StepRange HTMLInputElement::CreateStepRange(
    AnyStepHandling any_step_handling) const {
  return input_type_->CreateStepRange(any_step_handling);
}
