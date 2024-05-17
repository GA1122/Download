should_skip_file (CommonJob *common,
                  GFile     *file)
{
    if (common->skip_files != NULL)
    {
        return g_hash_table_lookup (common->skip_files, file) != NULL;
    }
    return FALSE;
}
