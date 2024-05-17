base::trace_event::TraceConfig GetBackgroundTracingConfig() {
  base::trace_event::TraceConfig trace_config(
      "-*,disabled-by-default-memory-infra",
      base::trace_event::RECORD_UNTIL_FULL);

  trace_config.EnableArgumentFilter();

  base::trace_event::TraceConfig::MemoryDumpConfig memory_config;
  memory_config.allowed_dump_modes.insert(
      base::trace_event::MemoryDumpLevelOfDetail::BACKGROUND);
  base::trace_event::TraceConfig::MemoryDumpConfig::Trigger trigger;
  trigger.min_time_between_dumps_ms = 1 << 30;
  trigger.level_of_detail =
      base::trace_event::MemoryDumpLevelOfDetail::BACKGROUND;
  trigger.trigger_type = base::trace_event::MemoryDumpType::PERIODIC_INTERVAL;
  memory_config.triggers.clear();
  memory_config.triggers.push_back(trigger);
  trace_config.ResetMemoryDumpConfig(memory_config);
  return trace_config;
}
