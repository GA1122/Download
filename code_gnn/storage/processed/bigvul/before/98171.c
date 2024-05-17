void AutoFillManager::GetBillingProfileSuggestions(
    FormStructure* form,
    const FormField& field,
    AutoFillType type,
    std::vector<string16>* values,
    std::vector<string16>* labels,
    std::vector<string16>* icons,
    std::vector<int>* unique_ids) {

  if (!FormIsHTTPS(form)) {
    GetProfileSuggestions(
        form, field, type, false, values, icons, labels, unique_ids);
    return;
  }

  std::vector<CreditCard*> matching_creditcards;
  std::vector<AutoFillProfile*> matching_profiles;

  for (std::vector<CreditCard*>::const_iterator cc =
           personal_data_->credit_cards().begin();
       cc != personal_data_->credit_cards().end(); ++cc) {
    int billing_address_id = (*cc)->billing_address_id();
    AutoFillProfile* billing_profile = NULL;

    string16 profile_field_value;

    for (std::vector<AutoFillProfile*>::const_iterator iter =
             personal_data_->profiles().begin();
         iter != personal_data_->profiles().end(); ++iter) {
      AutoFillProfile* profile = *iter;

      if (profile->unique_id() == billing_address_id &&
          !profile->GetFieldText(type).empty() &&
          StartsWith(profile->GetFieldText(type), field.value(), false)) {
        billing_profile = profile;
        break;
      }
    }

    if (!billing_profile)
      continue;

    matching_creditcards.push_back(*cc);
    matching_profiles.push_back(billing_profile);
  }

  std::vector<string16> inferred_labels;
  AutoFillProfile::CreateInferredLabels(&matching_profiles, &inferred_labels, 0,
                                        type.field_type());

  DCHECK_EQ(matching_profiles.size(), matching_creditcards.size());
  DCHECK_EQ(matching_profiles.size(), inferred_labels.size());

  for (size_t i = 0; i < matching_profiles.size(); ++i) {
    values->push_back(matching_profiles[i]->GetFieldText(type));
    string16 label = CombineLabelAndCreditCard(inferred_labels[i],
                                               matching_creditcards[i]);
    labels->push_back(label);
    icons->push_back(matching_creditcards[i]->type());
    unique_ids->push_back(PackIDs(matching_creditcards[i]->unique_id(),
                                  matching_profiles[i]->unique_id()));
  }
}
