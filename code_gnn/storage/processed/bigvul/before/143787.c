bool GlobalHistogramAllocator::CreateWithActiveFile(const FilePath& base_path,
                                                    const FilePath& active_path,
                                                    const FilePath& spare_path,
                                                    size_t size,
                                                    uint64_t id,
                                                    StringPiece name) {
  if (!base::ReplaceFile(active_path, base_path, nullptr))
    base::DeleteFile(base_path,  false);
  DCHECK(!base::PathExists(active_path));

  if (!spare_path.empty()) {
    base::ReplaceFile(spare_path, active_path, nullptr);
    DCHECK(!base::PathExists(spare_path));
  }

  return base::GlobalHistogramAllocator::CreateWithFile(active_path, size, id,
                                                        name);
}
