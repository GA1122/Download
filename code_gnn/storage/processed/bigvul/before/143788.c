bool GlobalHistogramAllocator::CreateWithActiveFileInDir(const FilePath& dir,
                                                         size_t size,
                                                         uint64_t id,
                                                         StringPiece name) {
  FilePath base_path, active_path, spare_path;
  ConstructFilePaths(dir, name, &base_path, &active_path, &spare_path);
  return CreateWithActiveFile(base_path, active_path, spare_path, size, id,
                              name);
}
