int64_t GetDefaultMemoryLimit() {
  const int kMegabyte = 1024 * 1024;

#if defined(OS_ANDROID)
  int64_t max_default_memory_limit = 128 * kMegabyte;
#else
  int64_t max_default_memory_limit = 512 * kMegabyte;
#endif

  if (base::SysInfo::IsLowEndDevice())
    max_default_memory_limit /= 8;

#if defined(OS_LINUX)
  base::FilePath shmem_dir;
  if (base::GetShmemTempDir(false, &shmem_dir)) {
    int64_t shmem_dir_amount_of_free_space =
        base::SysInfo::AmountOfFreeDiskSpace(shmem_dir);
    DCHECK_GT(shmem_dir_amount_of_free_space, 0);
    int64_t shmem_dir_amount_of_free_space_mb =
        shmem_dir_amount_of_free_space / kMegabyte;

    UMA_HISTOGRAM_CUSTOM_COUNTS("Memory.ShmemDir.AmountOfFreeSpace",
                                shmem_dir_amount_of_free_space_mb, 1,
                                4 * 1024,   
                                50);

    if (shmem_dir_amount_of_free_space_mb < 64) {
      LOG(WARNING) << "Less than 64MB of free space in temporary directory for "
                      "shared memory files: "
                   << shmem_dir_amount_of_free_space_mb;
    }

    max_default_memory_limit =
        std::min(max_default_memory_limit, shmem_dir_amount_of_free_space / 2);
  }
#endif

  return std::min(max_default_memory_limit,
                  base::SysInfo::AmountOfPhysicalMemory() / 4);
}
