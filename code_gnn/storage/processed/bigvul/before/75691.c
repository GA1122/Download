hfs_ext_compare_keys(HFS_INFO * hfs, uint32_t cnid,
    const hfs_btree_key_ext * key)
{
    TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
    uint32_t key_cnid;

    key_cnid = tsk_getu32(fs->endian, key->file_id);
    if (key_cnid < cnid)
        return -1;
    if (key_cnid > cnid)
        return 1;

     

     
    if (key->fork_type != HFS_EXT_KEY_TYPE_DATA)
        return 1;

     
    if (tsk_getu32(fs->endian, key->start_block) == 0)
        return 0;
    return 1;
}
