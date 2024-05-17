FilePath GlobalHistogramAllocator::ConstructFilePathForUploadDir(
    const FilePath& dir,
    StringPiece name,
    base::Time stamp,
    ProcessId pid) {
  return ConstructFilePath(
      dir,
      StringPrintf("%.*s-%lX-%lX", static_cast<int>(name.length()), name.data(),
                   static_cast<long>(stamp.ToTimeT()), static_cast<long>(pid)));
}
