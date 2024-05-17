void VerifyFormInteractionUkm(const ukm::TestAutoSetUkmRecorder& ukm_recorder,
                              const FormData& form,
                              const char* event_name,
                              const ExpectedUkmMetrics& expected_metrics) {
  auto entries = ukm_recorder.GetEntriesByName(event_name);

  EXPECT_LE(entries.size(), expected_metrics.size());
  for (size_t i = 0; i < expected_metrics.size() && i < entries.size(); i++) {
    ukm_recorder.ExpectEntrySourceHasUrl(entries[i],
                                         GURL(form.main_frame_origin.GetURL()));
    EXPECT_THAT(
        entries[i]->metrics,
        UnorderedPointwise(CompareMetricsIgnoringMillisecondsSinceFormParsed(),
                           expected_metrics[i]));
  }
}
