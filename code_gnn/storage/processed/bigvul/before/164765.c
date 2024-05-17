base::File OpenFileToShare(const base::FilePath& path,
                           base::MemoryMappedFile::Region* region) {
  return base::File(base::android::OpenApkAsset(path.value(), region));
}
