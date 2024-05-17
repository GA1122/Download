std::string GetHardwareClass() {
#if defined(OS_CHROMEOS)
  return base::SysInfo::GetLsbReleaseBoard();
#endif   
  return std::string();
}
