Core::Core() {
  handles_.reset(new HandleTable);
  base::trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
      handles_.get(), "MojoHandleTable", nullptr);
}
