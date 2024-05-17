bool GlobalHistogramAllocator::CreateSpareFileInDir(const FilePath& dir,
                                                    size_t size,
                                                    StringPiece name) {
  FilePath spare_path;
  ConstructFilePaths(dir, name, nullptr, nullptr, &spare_path);
  return CreateSpareFile(spare_path, size);
}
