void ReportSavePageResultHistogramAfterSave(const ClientId& client_id,
                                            SavePageResult result) {
  base::HistogramBase* histogram = base::LinearHistogram::FactoryGet(
      AddHistogramSuffix(client_id, "OfflinePages.SavePageResult"), 1,
      static_cast<int>(SavePageResult::RESULT_COUNT),
      static_cast<int>(SavePageResult::RESULT_COUNT) + 1,
      base::HistogramBase::kUmaTargetedHistogramFlag);
  histogram->Add(static_cast<int>(result));
}
