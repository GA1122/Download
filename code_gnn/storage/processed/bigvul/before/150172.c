void DumpBrowserHistograms(const base::FilePath& output_file) {
  std::string output_string(
      base::StatisticsRecorder::ToJSON(base::JSON_VERBOSITY_LEVEL_FULL));

  base::ScopedBlockingCall scoped_blocking_call(FROM_HERE,
                                                base::BlockingType::MAY_BLOCK);
  base::WriteFile(output_file, output_string.data(),
                  static_cast<int>(output_string.size()));
}
