void FileReaderLoader::AdjustReportedMemoryUsageToV8(int64_t usage) {
  if (!usage)
    return;
  memory_usage_reported_to_v8_ += usage;
  v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(usage);
  DCHECK_GE(memory_usage_reported_to_v8_, 0);
}
