void AutoFillManager::OnQueryFormFieldAutoFill(
    int query_id,
    const webkit_glue::FormData& form,
    const webkit_glue::FormField& field) {
  std::vector<string16> values;
  std::vector<string16> labels;
  std::vector<string16> icons;
  std::vector<int> unique_ids;

  RenderViewHost* host = NULL;
  FormStructure* form_structure = NULL;
  AutoFillField* autofill_field = NULL;
  if (GetHost(
          personal_data_->profiles(), personal_data_->credit_cards(), &host) &&
      FindCachedFormAndField(form, field, &form_structure, &autofill_field) &&
      form_structure->IsAutoFillable(false)) {
    AutoFillType type(autofill_field->type());
    bool is_filling_credit_card = (type.group() == AutoFillType::CREDIT_CARD);
    if (is_filling_credit_card) {
      GetCreditCardSuggestions(
          form_structure, field, type, &values, &labels, &icons, &unique_ids);
    } else {
      GetProfileSuggestions(
          form_structure, field, type, &values, &labels, &icons, &unique_ids);
    }

    DCHECK_EQ(values.size(), labels.size());
    DCHECK_EQ(values.size(), icons.size());
    DCHECK_EQ(values.size(), unique_ids.size());

    if (!values.empty()) {
      int warning = 0;
      if (!form_structure->IsAutoFillable(true))
        warning = IDS_AUTOFILL_WARNING_FORM_DISABLED;
      else if (is_filling_credit_card && !FormIsHTTPS(form_structure))
        warning = IDS_AUTOFILL_WARNING_INSECURE_CONNECTION;
      if (warning) {
        values.assign(1, l10n_util::GetStringUTF16(warning));
        labels.assign(1, string16());
        icons.assign(1, string16());
        unique_ids.assign(1, -1);
      } else {
        if (FormIsAutoFilled(form_structure, form, is_filling_credit_card)) {
          labels.assign(labels.size(), string16());
          icons.assign(icons.size(), string16());
        }

        RemoveDuplicateSuggestions(&values, &labels, &icons, &unique_ids);
      }
    }
  }

  tab_contents_->autocomplete_history_manager()->OnGetAutocompleteSuggestions(
      query_id, field.name(), field.value(), values, labels, icons, unique_ids);
}
