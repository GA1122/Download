int GetDPI(const PrintMsg_Print_Params* print_params) {
#if defined(OS_MACOSX)
  return kPointsPerInch;
#else
  return static_cast<int>(print_params->dpi);
#endif   
}
