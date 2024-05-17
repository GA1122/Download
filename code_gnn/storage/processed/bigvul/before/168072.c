void AutofillManager::OnLoadedServerPredictions(
    std::string response,
    const std::vector<std::string>& form_signatures) {
  std::vector<FormStructure*> queried_forms;
  for (const std::string& signature : base::Reversed(form_signatures)) {
    for (auto& cur_form : base::Reversed(form_structures())) {
      if (cur_form->FormSignatureAsStr() == signature) {
        queried_forms.push_back(cur_form.get());
        break;
      }
    }
  }
  std::reverse(queried_forms.begin(), queried_forms.end());

  if (queried_forms.empty())
    return;

  FormStructure::ParseQueryResponse(std::move(response), queried_forms);

  for (FormStructure* cur_form : queried_forms) {
    cur_form->LogQualityMetricsBasedOnAutocomplete(
        form_interactions_ukm_logger_.get());
  }

  driver()->PropagateAutofillPredictions(queried_forms);

  driver()->SendAutofillTypePredictionsToRenderer(queried_forms);
}
