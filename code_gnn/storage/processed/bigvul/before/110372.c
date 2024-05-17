uint32_t GetLiveVars(PP_Var live_vars[], uint32_t array_size) {
  std::vector<PP_Var> vars =
      PpapiGlobals::Get()->GetVarTracker()->GetLiveVars();
  for (size_t i = 0u;
       i < std::min(static_cast<size_t>(array_size), vars.size());
       ++i)
    live_vars[i] = vars[i];
  return vars.size();
}
