void RecordReporterStepHistogram(SwReporterUmaValue value) {
  UMA_HISTOGRAM_ENUMERATION("SoftwareReporter.Step", value, SW_REPORTER_MAX);
}
