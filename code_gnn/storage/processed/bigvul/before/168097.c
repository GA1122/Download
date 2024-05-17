void AutofillManager::TriggerRefill(const FormData& form,
                                    FormStructure* form_structure) {
  address_form_event_logger_->OnDidRefill();

  auto itr =
      filling_contexts_map_.find(form_structure->GetIdentifierForRefill());
  DCHECK(itr != filling_contexts_map_.end());
  FillingContext* filling_context = itr->second.get();

  DCHECK(!filling_context->attempted_refill);
  filling_context->attempted_refill = true;

  AutofillField* autofill_field = nullptr;
  for (const std::unique_ptr<AutofillField>& field : *form_structure) {
    if (field->unique_name() == filling_context->filled_field_name) {
      autofill_field = field.get();
      break;
    }
  }

  if (!autofill_field)
    return;

  FormFieldData field = *autofill_field;
  base::string16 cvc;
  FillOrPreviewDataModelForm(
      AutofillDriver::RendererFormDataAction::FORM_DATA_ACTION_FILL,
       -1, form, field, filling_context->temp_data_model,
       false, cvc, form_structure, autofill_field,
       true);
}
