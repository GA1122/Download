void HTMLInputElement::AppendToFormData(FormData& form_data) {
  if (input_type_->IsFormDataAppendable())
    input_type_->AppendToFormData(form_data);
}
