bool AutofillManager::ShouldTriggerRefill(const FormStructure& form_structure) {
  if (!base::FeatureList::IsEnabled(features::kAutofillDynamicForms))
    return false;

  address_form_event_logger_->OnDidSeeDynamicForm();

  auto itr =
      filling_contexts_map_.find(form_structure.GetIdentifierForRefill());
  if (itr == filling_contexts_map_.end())
    return false;

  address_form_event_logger_->OnDidSeeFillableDynamicForm();

  FillingContext* filling_context = itr->second.get();
  base::TimeTicks now = base::TimeTicks::Now();
  base::TimeDelta delta = now - filling_context->original_fill_time;

  if (filling_context->attempted_refill &&
      delta.InMilliseconds() < kLimitBeforeRefillMs) {
    address_form_event_logger_->OnSubsequentRefillAttempt();
  }

  return !filling_context->attempted_refill &&
         delta.InMilliseconds() < kLimitBeforeRefillMs;
}
