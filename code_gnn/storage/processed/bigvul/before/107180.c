void AutoFillManager::OnFillAutoFillFormData(int query_id,
                                             const FormData& form,
                                             const FormField& field,
                                             int unique_id) {
  const std::vector<AutoFillProfile*>& profiles = personal_data_->profiles();
  const std::vector<CreditCard*>& credit_cards = personal_data_->credit_cards();
  RenderViewHost* host = NULL;
  FormStructure* form_structure = NULL;
  AutoFillField* autofill_field = NULL;
  if (!GetHost(profiles, credit_cards, &host) ||
      !FindCachedFormAndField(form, field, &form_structure, &autofill_field))
    return;

  DCHECK(host);
  DCHECK(form_structure);
  DCHECK(autofill_field);

  std::string cc_guid;
  std::string profile_guid;
  UnpackGUIDs(unique_id, &cc_guid, &profile_guid);
  DCHECK(!guid::IsValidGUID(cc_guid) || !guid::IsValidGUID(profile_guid));

  const AutoFillProfile* profile = NULL;
  if (guid::IsValidGUID(profile_guid)) {
    for (std::vector<AutoFillProfile*>::const_iterator iter = profiles.begin();
         iter != profiles.end(); ++iter) {
      if ((*iter)->guid() == profile_guid) {
        profile = *iter;
        break;
      }
    }
    DCHECK(profile);
  }

  const CreditCard* credit_card = NULL;
  if (guid::IsValidGUID(cc_guid)) {
    for (std::vector<CreditCard*>::const_iterator iter = credit_cards.begin();
         iter != credit_cards.end(); ++iter) {
      if ((*iter)->guid() == cc_guid) {
        credit_card = *iter;
        break;
      }
    }
    DCHECK(credit_card);
  }

  if (!profile && !credit_card)
    return;

  FormData result = form;

  if (FormIsAutoFilled(form_structure, form, (credit_card != NULL))) {
    for (std::vector<FormField>::iterator iter = result.fields.begin();
         iter != result.fields.end(); ++iter) {
      if ((*iter) == field) {
        AutoFillType autofill_type(autofill_field->type());
        if (credit_card &&
            autofill_type.group() == AutoFillType::CREDIT_CARD) {
          FillCreditCardFormField(credit_card, autofill_type, &(*iter));
        } else if (profile &&
                   autofill_type.group() != AutoFillType::CREDIT_CARD) {
          FillFormField(profile, autofill_type, &(*iter));
        }
        break;
      }
    }

    host->Send(new AutoFillMsg_FormDataFilled(
        host->routing_id(), query_id, result));
    return;
  }

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
    } else if (profile &&
               autofill_type.group() != AutoFillType::CREDIT_CARD) {
      FillFormField(profile, autofill_type, &result.fields[j]);
    }

    ++i;
  }
  autofilled_forms_signatures_.push_front(form_structure->FormSignature());

  host->Send(new AutoFillMsg_FormDataFilled(
      host->routing_id(), query_id, result));
}
