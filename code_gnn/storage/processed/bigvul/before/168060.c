bool AutofillManager::MaybeStartVoteUploadProcess(
    std::unique_ptr<FormStructure> form_structure,
    const TimeTicks& timestamp,
    bool observed_submission) {
  if (!personal_data_)
    return false;

  const std::vector<AutofillProfile*>& profiles = personal_data_->GetProfiles();
  if (observed_submission && form_structure->IsAutofillable()) {
    AutofillMetrics::LogNumberOfProfilesAtAutofillableFormSubmission(
        personal_data_->GetProfiles().size());
  }
  const std::vector<CreditCard*>& credit_cards =
      personal_data_->GetCreditCards();
  if (profiles.empty() && credit_cards.empty())
    return false;
  std::vector<AutofillProfile> copied_profiles;
  copied_profiles.reserve(profiles.size());
  for (const AutofillProfile* profile : profiles)
    copied_profiles.push_back(*profile);

  std::vector<CreditCard> copied_credit_cards;
  copied_credit_cards.reserve(credit_cards.size());
  for (const CreditCard* card : credit_cards)
    copied_credit_cards.push_back(*card);

  FormStructure* raw_form = form_structure.get();
  TimeTicks loaded_timestamp = forms_loaded_timestamps_[raw_form->ToFormData()];
  base::PostTaskWithTraitsAndReply(
      FROM_HERE, {base::MayBlock(), base::TaskPriority::BACKGROUND},
      base::BindOnce(&AutofillManager::DeterminePossibleFieldTypesForUpload,
                     copied_profiles, copied_credit_cards, app_locale_,
                     raw_form),
      base::BindOnce(&AutofillManager::UploadFormDataAsyncCallback,
                     weak_ptr_factory_.GetWeakPtr(),
                     base::Owned(form_structure.release()), loaded_timestamp,
                     initial_interaction_timestamp_, timestamp,
                     observed_submission));
  return true;
}
