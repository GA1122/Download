  void FillAutofillFormData(int query_id,
                            const FormData& form,
                            const FormFieldData& field,
                            int unique_id) {
    autofill_manager_->FillOrPreviewForm(AutofillDriver::FORM_DATA_ACTION_FILL,
                                         query_id, form, field, unique_id);
  }
