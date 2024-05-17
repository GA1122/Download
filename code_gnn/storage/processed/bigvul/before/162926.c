  void EnableMemoryInfraTracing() {
    TraceLog::GetInstance()->SetEnabled(
        TraceConfig(MemoryDumpManager::kTraceCategory, ""),
        TraceLog::RECORDING_MODE);
  }
