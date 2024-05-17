void RecordSRTPromptHistogram(SRTPromptHistogramValue value) {
  UMA_HISTOGRAM_ENUMERATION("SoftwareReporter.PromptUsage", value,
                            SRT_PROMPT_MAX);
}
