void AutofillManager::FillOrPreviewDataModelForm(
    AutofillDriver::RendererFormDataAction action,
    int query_id,
    const FormData& form,
    const FormFieldData& field,
    const AutofillDataModel& data_model,
    bool is_credit_card,
    const base::string16& cvc,
    FormStructure* form_structure,
    AutofillField* autofill_field,
    bool is_refill) {
  DCHECK(form_structure);
  DCHECK(autofill_field);

  FormData result = form;

  if (base::FeatureList::IsEnabled(kAutofillRationalizeFieldTypePredictions)) {
    form_structure->RationalizePhoneNumbersInSection(autofill_field->section);
  }

  DCHECK_EQ(form_structure->field_count(), form.fields.size());

  FillingContext* filling_context = nullptr;
  auto itr =
      filling_contexts_map_.find(form_structure->GetIdentifierForRefill());
  if (itr != filling_contexts_map_.end())
    filling_context = itr->second.get();
  bool could_attempt_refill =
      base::FeatureList::IsEnabled(features::kAutofillDynamicForms) &&
      filling_context != nullptr && !filling_context->attempted_refill &&
      !is_refill && !is_credit_card;

  for (size_t i = 0; i < form_structure->field_count(); ++i) {
    if (form_structure->field(i)->section != autofill_field->section)
      continue;

    if (form_structure->field(i)->only_fill_when_focused() &&
        !form_structure->field(i)->SameFieldAs(field)) {
      continue;
    }

    DCHECK(form_structure->field(i)->SameFieldAs(result.fields[i]));

    AutofillField* cached_field = form_structure->field(i);
    FieldTypeGroup field_group_type = cached_field->Type().group();

    if (!cached_field->IsVisible()) {
      bool skip = result.fields[i].form_control_type != "select-one";
      form_interactions_ukm_logger_->LogHiddenRepresentationalFieldSkipDecision(
          *form_structure, *cached_field, skip);
      if (skip)
        continue;
    }

    if (result.fields[i].is_autofilled && !cached_field->SameFieldAs(field) &&
        !is_refill) {
      continue;
    }

    if (field_group_type == NO_GROUP)
      continue;

    if (is_refill &&
        !base::ContainsKey(filling_context->type_groups_originally_filled,
                           field_group_type)) {
      continue;
    }

    if (IsCreditCardExpirationType(cached_field->Type().GetStorableType()) &&
        static_cast<const CreditCard*>(&data_model)
            ->IsExpired(AutofillClock::Now())) {
      continue;
    }

    if (could_attempt_refill)
      filling_context->type_groups_originally_filled.insert(field_group_type);

    bool should_notify = !is_credit_card &&
                         (result.fields[i].SameFieldAs(field) ||
                          result.fields[i].form_control_type == "select-one" ||
                          result.fields[i].value.empty());

    FillFieldWithValue(cached_field, data_model, &result.fields[i],
                       should_notify, cvc);
    if (result.fields[i].is_autofilled)
      result.fields[i].section = form_structure->field(i)->section;

    if (!cached_field->IsVisible() && result.fields[i].is_autofilled) {
      AutofillMetrics::LogHiddenOrPresentationalSelectFieldsFilled();
    }
  }

  autofilled_form_signatures_.push_front(form_structure->FormSignatureAsStr());
  if (autofilled_form_signatures_.size() > kMaxRecentFormSignaturesToRemember)
    autofilled_form_signatures_.pop_back();

  if (action == AutofillDriver::FORM_DATA_ACTION_FILL && !is_refill)
    personal_data_->RecordUseOf(data_model);

  driver()->SendFormDataToRenderer(query_id, action, result);
}
