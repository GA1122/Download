void PasswordAutofillAgent::LogFirstFillingResult(
    const PasswordFormFillData& form_data,
    FillingResult result) {
  if (recorded_first_filling_result_)
    return;
  UMA_HISTOGRAM_ENUMERATION("PasswordManager.FirstRendererFillingResult",
                            result);
  DCHECK(form_data.has_renderer_ids);
  GetPasswordManagerDriver()->LogFirstFillingResult(
      form_data.form_renderer_id, base::strict_cast<int32_t>(result));
  recorded_first_filling_result_ = true;
}
