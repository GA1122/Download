std::string GenerateIpcChannelName(void* client) {
  return base::StringPrintf("%d.%p.%d",
                            base::GetCurrentProcId(), client,
                            base::RandInt(0, std::numeric_limits<int>::max()));
}
