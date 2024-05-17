void AutofillManager::OnFillAutofillFormData(int query_id,
                                             const FormData& form,
                                             const FormField& field,
                                             int unique_id) {
  const std::vector<AutofillProfile*>& profiles = personal_data_->profiles();
  const std::vector<CreditCard*>& credit_cards = personal_data_->credit_cards();
  RenderViewHost* host = NULL;
  FormStructure* form_structure = NULL;
  AutofillField* autofill_field = NULL;
  if (!GetHost(profiles, credit_cards, &host) ||
      !FindCachedFormAndField(form, field, &form_structure, &autofill_field))
    return;

  DCHECK(host);
  DCHECK(form_structure);
  DCHECK(autofill_field);

  GUIDPair cc_guid;
  GUIDPair profile_guid;
  UnpackGUIDs(unique_id, &cc_guid, &profile_guid);
  DCHECK(!guid::IsValidGUID(cc_guid.first) ||
         !guid::IsValidGUID(profile_guid.first));

  const AutofillProfile* profile = NULL;
  if (guid::IsValidGUID(profile_guid.first)) {
    for (std::vector<AutofillProfile*>::const_iterator iter = profiles.begin();
         iter != profiles.end(); ++iter) {
      if ((*iter)->guid() == profile_guid.first) {
        profile = *iter;
        break;
      }
    }
    DCHECK(profile);
  }

  const CreditCard* credit_card = NULL;
  if (guid::IsValidGUID(cc_guid.first)) {
    for (std::vector<CreditCard*>::const_iterator iter = credit_cards.begin();
         iter != credit_cards.end(); ++iter) {
      if ((*iter)->guid() == cc_guid.first) {
        credit_card = *iter;
        break;
      }
    }
    DCHECK(credit_card);
  }

  if (!profile && !credit_card)
    return;

  size_t section_start, section_end;
  FindSectionBounds(*form_structure, *autofill_field, (credit_card != NULL),
                    &section_start, &section_end);

  FormData result = form;

  if (SectionIsAutofilled(form_structure, form, section_start, section_end)) {
    for (std::vector<FormField>::iterator iter = result.fields.begin();
         iter != result.fields.end(); ++iter) {
      if ((*iter) == field) {
        AutofillFieldType field_type = autofill_field->type();
        if (profile) {
          DCHECK_NE(AutofillType::CREDIT_CARD,
                    AutofillType(field_type).group());
          FillFormField(profile, field_type, profile_guid.second, &(*iter));
        } else {
          DCHECK_EQ(AutofillType::CREDIT_CARD,
                    AutofillType(field_type).group());
          FillCreditCardFormField(credit_card, field_type, &(*iter));
        }
        break;
      }
    }

    host->Send(new AutofillMsg_FormDataFilled(host->routing_id(), query_id,
                                              result));
    return;
  }

  for (size_t i = section_start, j = 0;
       i < section_end && j < result.fields.size();
       j++) {
    size_t k = i;

    while (k < section_end && *form_structure->field(k) != result.fields[j]) {
      k++;
    }

    if (k >= section_end)
      continue;

    AutofillFieldType field_type = form_structure->field(k)->type();
    FieldTypeGroup field_group_type = AutofillType(field_type).group();
    if (field_group_type != AutofillType::NO_GROUP) {
      if (profile) {
        DCHECK_NE(AutofillType::CREDIT_CARD, field_group_type);
        if (result.fields[j] == field) {
          FillFormField(profile, field_type, profile_guid.second,
                        &result.fields[j]);
        } else {
          FillFormField(profile, field_type, 0, &result.fields[j]);
        }
      } else {
        DCHECK_EQ(AutofillType::CREDIT_CARD, field_group_type);
        FillCreditCardFormField(credit_card, field_type, &result.fields[j]);
      }
    }

    ++i;
  }

  autofilled_form_signatures_.push_front(form_structure->FormSignature());
  if (autofilled_form_signatures_.size() > kMaxRecentFormSignaturesToRemember)
    autofilled_form_signatures_.pop_back();

  host->Send(new AutofillMsg_FormDataFilled(
      host->routing_id(), query_id, result));
}
