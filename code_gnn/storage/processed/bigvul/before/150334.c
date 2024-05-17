void PasswordAutofillAgent::SetLastUpdatedFormAndField(
    const WebFormElement& form,
    const WebFormControlElement& input) {
  last_updated_form_renderer_id_ = form.IsNull()
                                       ? FormData::kNotSetFormRendererId
                                       : form.UniqueRendererFormId();
  last_updated_field_renderer_id_ = input.IsNull()
                                        ? FormData::kNotSetFormRendererId
                                        : input.UniqueRendererFormControlId();
}
