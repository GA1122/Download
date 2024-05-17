bool AutoFillManager::FillAutoFillFormData(int query_id,
                                           const FormData& form,
                                           int unique_id) {
  if (!IsAutoFillEnabled())
    return false;

  RenderViewHost* host = tab_contents_->render_view_host();
  if (!host)
    return false;

  const std::vector<AutoFillProfile*>& profiles = personal_data_->profiles();
  const std::vector<CreditCard*>& credit_cards = personal_data_->credit_cards();

  if (profiles.empty() && credit_cards.empty())
    return false;

  FormData result = form;
  FormStructure* form_structure = NULL;
  for (std::vector<FormStructure*>::const_iterator iter =
           form_structures_.begin();
       iter != form_structures_.end(); ++iter) {
    if (**iter == form) {
      form_structure = *iter;
      break;
    }
  }

  if (!form_structure)
    return false;

  if (!form_structure->autofill_count())
    return false;

  int cc_id = 0;
  int profile_id = 0;
  UnpackIDs(unique_id, &cc_id, &profile_id);

  const AutoFillProfile* profile = NULL;
  if (profile_id != 0) {
    for (std::vector<AutoFillProfile*>::const_iterator iter = profiles.begin();
         iter != profiles.end(); ++iter) {
      if ((*iter)->unique_id() == profile_id) {
        profile = *iter;
        break;
      }
    }
  }

  const CreditCard* credit_card = NULL;
  if (cc_id != 0) {
    for (std::vector<CreditCard*>::const_iterator iter = credit_cards.begin();
         iter != credit_cards.end(); ++iter) {
      if ((*iter)->unique_id() == cc_id) {
        credit_card = *iter;
        break;
      }
    }
  }

  if (!profile && !credit_card)
    return false;

  for (size_t i = 0, j = 0;
       i < form_structure->field_count() && j < result.fields.size();
       j++) {
    size_t k = i;

    while (k < form_structure->field_count() &&
           *form_structure->field(k) != result.fields[j]) {
      k++;
    }

    if (k >= form_structure->field_count())
      continue;

    const AutoFillField* field = form_structure->field(k);
    AutoFillType autofill_type(field->type());
    if (credit_card &&
        autofill_type.group() == AutoFillType::CREDIT_CARD) {
      FillCreditCardFormField(credit_card, autofill_type, &result.fields[j]);
    } else if (credit_card &&
               autofill_type.group() == AutoFillType::ADDRESS_BILLING) {
      FillBillingFormField(credit_card, autofill_type, &result.fields[j]);
    } else if (profile) {
      FillFormField(profile, autofill_type, &result.fields[j]);
    }

    ++i;
  }
  autofilled_forms_signatures_.push_front(form_structure->FormSignature());

  host->AutoFillFormDataFilled(query_id, result);
  return true;
}
