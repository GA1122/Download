hfs_find_highest_inum_cb(HFS_INFO * hfs, int8_t level_type,
    const hfs_btree_key_cat * cur_key,
    TSK_OFF_T key_off, void *ptr)
{
    *((TSK_INUM_T*) ptr) = tsk_getu32(hfs->fs_info.endian, cur_key->parent_cnid);
    return HFS_BTREE_CB_IDX_LT;
}
