  void WriteMetricsFile(const base::FilePath& path,
                        base::PersistentHistogramAllocator* metrics) {
    base::File writer(path,
                      base::File::FLAG_CREATE_ALWAYS | base::File::FLAG_WRITE);
    DCHECK(writer.IsValid()) << path.value();
    size_t file_size = create_large_files_ ? metrics->size() : metrics->used();
    int written = writer.Write(0, (const char*)metrics->data(), file_size);
    DCHECK_EQ(static_cast<int>(file_size), written);
  }
