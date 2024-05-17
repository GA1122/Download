tar_sparse_extract_region (struct tar_sparse_file *file, size_t i)
{
  if (file->optab->extract_region)
    return file->optab->extract_region (file, i);
  return false;
}
