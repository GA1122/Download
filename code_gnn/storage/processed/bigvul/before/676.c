tar_sparse_fixup_header (struct tar_sparse_file *file)
{
  if (file->optab->fixup_header)
    return file->optab->fixup_header (file);
  return true;
}
