bool AutofillManager::ParseFormInternal(const FormData& form,
                                        const FormStructure* cached_form,
                                        FormStructure** parsed_form_structure) {
  if (ParseForm(form, cached_form, parsed_form_structure)) {
    (*parsed_form_structure)
        ->DetermineHeuristicTypes(client_->GetUkmRecorder(),
                                  client_->GetUkmSourceId());
    return true;
  }
  return false;
}
