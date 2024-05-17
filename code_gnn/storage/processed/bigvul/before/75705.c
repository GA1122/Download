hfs_get_default_attr_type(const TSK_FS_FILE * a_file)
{
    TSK_INUM_T inum = a_file->meta->addr;
    if (inum == 3 ||             
        inum == 4 ||             
        inum == 5 ||             
        inum == 6 ||             
        inum == 7 ||             
        inum == 8 ||             
        inum == 14 ||            
        inum == 15)              
        return TSK_FS_ATTR_TYPE_DEFAULT;
    if (a_file->meta->type == TSK_FS_META_TYPE_REG ||
        a_file->meta->type == TSK_FS_META_TYPE_LNK)
        return TSK_FS_ATTR_TYPE_HFS_DATA;

    return TSK_FS_ATTR_TYPE_DEFAULT;
}
