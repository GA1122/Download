  CreateMetricsFileWithHistograms(
      const base::FilePath& file_path,
      base::Time write_time,
      int histogram_count,
      const std::function<void(base::PersistentHistogramAllocator*)>&
          callback) {
    base::GlobalHistogramAllocator::CreateWithLocalMemory(
        create_large_files_ ? kLargeFileSize : kSmallFileSize,
        0, kMetricsName);

    CreateGlobalHistograms(histogram_count);

    std::unique_ptr<base::PersistentHistogramAllocator> histogram_allocator =
        base::GlobalHistogramAllocator::ReleaseForTesting();
    callback(histogram_allocator.get());

    WriteMetricsFileAtTime(file_path, histogram_allocator.get(), write_time);
    return histogram_allocator;
  }
