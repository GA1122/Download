bool PPAPITestBase::GetHTTPDocumentRoot(FilePath* document_root) {
  FilePath exe_dir = CommandLine::ForCurrentProcess()->GetProgram().DirName();
  FilePath src_dir;
  if (!PathService::Get(base::DIR_SOURCE_ROOT, &src_dir))
    return false;

  if (!exe_dir.IsAbsolute()) file_util::AbsolutePath(&exe_dir);
  if (!src_dir.IsAbsolute()) file_util::AbsolutePath(&src_dir);
  if (!exe_dir.IsAbsolute())
    return false;
  if (!src_dir.IsAbsolute())
    return false;

  size_t match, exe_size, src_size;
  std::vector<FilePath::StringType> src_parts, exe_parts;

  exe_dir.GetComponents(&exe_parts);
  src_dir.GetComponents(&src_parts);
  exe_size = exe_parts.size();
  src_size = src_parts.size();
  for (match = 0; match < exe_size && match < src_size; ++match) {
    if (exe_parts[match] != src_parts[match])
      break;
  }
  for (size_t tmp_itr = match; tmp_itr < src_size; ++tmp_itr) {
    *document_root = document_root->Append(FILE_PATH_LITERAL(".."));
  }
  for (; match < exe_size; ++match) {
    *document_root = document_root->Append(exe_parts[match]);
  }
  return true;
}
