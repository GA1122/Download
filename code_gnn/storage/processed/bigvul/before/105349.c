void AutofillManager::OnQueryFormFieldAutofill(
    int query_id,
    const webkit_glue::FormData& form,
    const webkit_glue::FormField& field) {
  std::vector<string16> values;
  std::vector<string16> labels;
  std::vector<string16> icons;
  std::vector<int> unique_ids;

  RenderViewHost* host = NULL;
  FormStructure* form_structure = NULL;
  AutofillField* autofill_field = NULL;
  if (GetHost(
          personal_data_->profiles(), personal_data_->credit_cards(), &host) &&
      FindCachedFormAndField(form, field, &form_structure, &autofill_field) &&
      form_structure->IsAutofillable(false)) {
    AutofillFieldType type = autofill_field->type();
    bool is_filling_credit_card =
        (AutofillType(type).group() == AutofillType::CREDIT_CARD);
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
      if (!form_structure->IsAutofillable(true))
        warning = IDS_AUTOFILL_WARNING_FORM_DISABLED;
      else if (is_filling_credit_card && !FormIsHTTPS(form_structure))
        warning = IDS_AUTOFILL_WARNING_INSECURE_CONNECTION;
      if (warning) {
        values.assign(1, l10n_util::GetStringUTF16(warning));
        labels.assign(1, string16());
        icons.assign(1, string16());
        unique_ids.assign(1, -1);
      } else {
        size_t section_start, section_end;
        FindSectionBounds(*form_structure, *autofill_field,
                          is_filling_credit_card, &section_start, &section_end);

        bool section_is_autofilled = SectionIsAutofilled(form_structure,
                                                         form,
                                                         section_start,
                                                         section_end);
        if (section_is_autofilled) {
          labels.assign(labels.size(), string16());
          icons.assign(icons.size(), string16());
        }

        if (!is_filling_credit_card)
          RemoveDuplicateSuggestions(&values, &labels, &icons, &unique_ids);

        if (!has_logged_address_suggestions_count_ && !section_is_autofilled) {
          metric_logger_->LogAddressSuggestionsCount(values.size());
          has_logged_address_suggestions_count_ = true;
        }
      }
    }
  }

  tab_contents_wrapper_->autocomplete_history_manager()->
      OnGetAutocompleteSuggestions(
          query_id, field.name, field.value, values, labels, icons, unique_ids);
}
