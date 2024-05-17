static void RecordPurgeAndSuspendMemoryGrowthKB(const char* basename,
                                                const char* suffix,
                                                int memory_usage) {
  std::string histogram_name = base::StringPrintf("%s.%s", basename, suffix);
  base::UmaHistogramMemoryKB(histogram_name, memory_usage);
}
