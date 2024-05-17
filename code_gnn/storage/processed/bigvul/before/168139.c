void AppendFieldTypeUkm(const FormData& form,
                        const std::vector<ServerFieldType>& heuristic_types,
                        const std::vector<ServerFieldType>& server_types,
                        const std::vector<ServerFieldType>& actual_types,
                        ExpectedUkmMetrics* expected_metrics) {
  ASSERT_EQ(heuristic_types.size(), form.fields.size());
  ASSERT_EQ(server_types.size(), form.fields.size());
  ASSERT_EQ(actual_types.size(), form.fields.size());
  int64_t form_signature = Collapse(CalculateFormSignature(form));
  int64_t metric_type = static_cast<int64_t>(AutofillMetrics::TYPE_SUBMISSION);
  std::vector<int64_t> prediction_sources{
      AutofillMetrics::PREDICTION_SOURCE_HEURISTIC,
      AutofillMetrics::PREDICTION_SOURCE_SERVER,
      AutofillMetrics::PREDICTION_SOURCE_OVERALL};
  for (size_t i = 0; i < form.fields.size(); ++i) {
    const FormFieldData& field = form.fields[i];
    int64_t field_signature = Collapse(CalculateFieldSignatureForField(field));
    for (int64_t source : prediction_sources) {
      int64_t predicted_type = static_cast<int64_t>(
          (source == AutofillMetrics::PREDICTION_SOURCE_SERVER
               ? server_types
               : heuristic_types)[i]);
      int64_t actual_type = static_cast<int64_t>(actual_types[i]);
      expected_metrics->push_back(
          {{UkmSuggestionFilledType::kMillisecondsSinceFormParsedName, 0},
           {UkmFieldFillStatusType::kFormSignatureName, form_signature},
           {UkmFieldFillStatusType::kFieldSignatureName, field_signature},
           {UkmFieldFillStatusType::kValidationEventName, metric_type},
           {UkmFieldTypeValidationType::kPredictionSourceName, source},
           {UkmFieldTypeValidationType::kPredictedTypeName, predicted_type},
           {UkmFieldTypeValidationType::kActualTypeName, actual_type}});
    }
  }
}
