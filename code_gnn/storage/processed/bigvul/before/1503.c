static size_t v9fs_readdir_data_size(V9fsString *name)
{
     
    return 24 + v9fs_string_size(name);
}
