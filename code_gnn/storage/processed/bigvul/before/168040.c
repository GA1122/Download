void AutofillManager::FillOrPreviewProfileForm(
    AutofillDriver::RendererFormDataAction action,
    int query_id,
    const FormData& form,
    const FormFieldData& field,
    const AutofillProfile& profile) {
  FormStructure* form_structure = nullptr;
  AutofillField* autofill_field = nullptr;
  if (!GetCachedFormAndField(form, field, &form_structure, &autofill_field))
    return;
  if (action == AutofillDriver::FORM_DATA_ACTION_FILL) {
    address_form_event_logger_->OnDidFillSuggestion(profile, *form_structure,
                                                    *autofill_field);

    if (base::FeatureList::IsEnabled(features::kAutofillDynamicForms) &&
        !form_structure->GetIdentifierForRefill().empty()) {
      auto& entry =
          filling_contexts_map_[form_structure->GetIdentifierForRefill()];
      auto filling_context = std::make_unique<FillingContext>();
      filling_context->temp_data_model = profile;
      filling_context->filled_field_name = autofill_field->unique_name();
      filling_context->original_fill_time = base::TimeTicks::Now();
      entry = std::move(filling_context);
    }
  }

  FillOrPreviewDataModelForm(
      action, query_id, form, field, profile,  false,
       base::string16(), form_structure, autofill_field);
}
