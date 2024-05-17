void GenerateSafeFileName(const std::string& mime_type,
                          bool ignore_extension,
                          FilePath* file_path) {
  EnsureSafeExtension(mime_type, ignore_extension, file_path);

#if defined(OS_WIN)
  FilePath::StringType leaf_name = file_path->BaseName().value();
  DCHECK(!leaf_name.empty());
  if (IsReservedName(leaf_name)) {
    leaf_name = FilePath::StringType(FILE_PATH_LITERAL("_")) + leaf_name;
    *file_path = file_path->DirName();
    if (file_path->value() == FilePath::kCurrentDirectory) {
      *file_path = FilePath(leaf_name);
    } else {
      *file_path = file_path->Append(leaf_name);
    }
  }
#endif
}
