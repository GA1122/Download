void VerifyDeveloperEngagementUkm(
    const ukm::TestAutoSetUkmRecorder& ukm_recorder,
    const FormData& form,
    const bool is_for_credit_card,
    const std::set<FormType>& form_types,
    const std::vector<int64_t>& expected_metric_values) {
  int expected_metric_value = 0;
  for (const auto it : expected_metric_values)
    expected_metric_value |= 1 << it;

  auto entries =
      ukm_recorder.GetEntriesByName(UkmDeveloperEngagementType::kEntryName);
  EXPECT_EQ(1u, entries.size());
  for (const auto* const entry : entries) {
    ukm_recorder.ExpectEntrySourceHasUrl(entry,
                                         GURL(form.main_frame_origin.GetURL()));
    EXPECT_EQ(4u, entry->metrics.size());
    ukm_recorder.ExpectEntryMetric(
        entry, UkmDeveloperEngagementType::kDeveloperEngagementName,
        expected_metric_value);
    ukm_recorder.ExpectEntryMetric(
        entry, UkmDeveloperEngagementType::kIsForCreditCardName,
        is_for_credit_card);
    ukm_recorder.ExpectEntryMetric(
        entry, UkmDeveloperEngagementType::kFormTypesName,
        AutofillMetrics::FormTypesToBitVector(form_types));
    ukm_recorder.ExpectEntryMetric(
        entry, UkmDeveloperEngagementType::kFormSignatureName,
        Collapse(CalculateFormSignature(form)));
  }
}
