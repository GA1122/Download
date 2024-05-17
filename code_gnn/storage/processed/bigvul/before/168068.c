void AutofillManager::OnFormsParsed(
    const std::vector<FormStructure*>& form_structures,
    const base::TimeTicks timestamp) {
  DCHECK(!form_structures.empty());

  form_interactions_ukm_logger_->OnFormsParsed(
      form_structures[0]->ToFormData().main_frame_origin.GetURL(),
      client_->GetUkmSourceId());

  std::vector<FormStructure*> non_queryable_forms;
  std::vector<FormStructure*> queryable_forms;
  std::set<FormType> form_types;
  for (FormStructure* form_structure : form_structures) {
    form_structure->DetermineHeuristicTypes(client_->GetUkmRecorder(),
                                            client_->GetUkmSourceId());
    forms_loaded_timestamps_[form_structure->ToFormData()] = timestamp;
    std::set<FormType> current_form_types = form_structure->GetFormTypes();
    form_types.insert(current_form_types.begin(), current_form_types.end());
    if (form_structure->ShouldBeQueried())
      queryable_forms.push_back(form_structure);
    else
      non_queryable_forms.push_back(form_structure);

    if (ShouldTriggerRefill(*form_structure)) {
      auto itr =
          filling_contexts_map_.find(form_structure->GetIdentifierForRefill());
      DCHECK(itr != filling_contexts_map_.end());
      FillingContext* filling_context = itr->second.get();

      if (filling_context->on_refill_timer.IsRunning())
        filling_context->on_refill_timer.AbandonAndStop();

      filling_context->on_refill_timer.Start(
          FROM_HERE,
          base::TimeDelta::FromMilliseconds(kWaitTimeForDynamicFormsMs),
          base::BindRepeating(&AutofillManager::TriggerRefill,
                              weak_ptr_factory_.GetWeakPtr(),
                              form_structure->ToFormData(), form_structure));
    }
  }

  if (!queryable_forms.empty() || !non_queryable_forms.empty()) {
    AutofillMetrics::LogUserHappinessMetric(
        AutofillMetrics::FORMS_LOADED, form_types,
        client_->GetSecurityLevelForUmaHistograms());

#if defined(OS_IOS)
    KeyboardAccessoryMetricsLogger::OnFormsLoaded();
#endif
  }

#if defined(OS_ANDROID) || defined(OS_IOS)
  if (autofill_assistant_.CanShowCreditCardAssist(
          AutofillHandler::form_structures())) {
    const std::vector<CreditCard*> cards =
        personal_data_->GetCreditCardsToSuggest(
            client_->AreServerCardsSupported());
    if (!cards.empty() && !cards.front()->IsExpired(AutofillClock::Now()))
      autofill_assistant_.ShowAssistForCreditCard(*cards.front());
  }
#endif

  driver()->SendAutofillTypePredictionsToRenderer(non_queryable_forms);
  driver()->SendAutofillTypePredictionsToRenderer(queryable_forms);

  if (!queryable_forms.empty() && download_manager_) {
    download_manager_->StartQueryRequest(queryable_forms);
  }
}
