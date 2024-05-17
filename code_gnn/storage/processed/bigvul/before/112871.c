int64 GetAmountOfFreeDiskSpace() {
  if (global_free_disk_getter_for_testing)
    return global_free_disk_getter_for_testing->AmountOfFreeDiskSpace();

  return base::SysInfo::AmountOfFreeDiskSpace(
      FilePath::FromUTF8Unsafe(GetHomeDirectory()));
}
