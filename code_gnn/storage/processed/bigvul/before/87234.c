hfs_attr_walk_compressed_rsrc(const TSK_FS_ATTR * fs_attr,
    int flags, TSK_FS_FILE_WALK_CB a_action, void *ptr,
    int (*read_block_table)(const TSK_FS_ATTR *rAttr,
                            CMP_OFFSET_ENTRY** offsetTableOut,
                            uint32_t* tableSizeOut,
                            uint32_t* tableOffsetOut),
    int (*decompress_block)(char* rawBuf,
                            uint32_t len,
                            char* uncBuf,
                            uint64_t* uncLen))
{
    TSK_FS_INFO *fs;
    TSK_FS_FILE *fs_file;
    const TSK_FS_ATTR *rAttr;    
    char *rawBuf = NULL;                
    char *uncBuf = NULL;                
    uint32_t offsetTableOffset;
    uint32_t offsetTableSize;          
    CMP_OFFSET_ENTRY *offsetTable = NULL;
    size_t indx;                 
    TSK_OFF_T off = 0;           

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "%s:  Entered, because this is a compressed file with compressed data in the resource fork\n", __func__);

    tsk_error_reset();
    if ((fs_attr == NULL) || (fs_attr->fs_file == NULL)
        || (fs_attr->fs_file->meta == NULL)
        || (fs_attr->fs_file->fs_info == NULL)) {
        tsk_error_set_errno(TSK_ERR_FS_ARG);
        tsk_error_set_errstr("%s: Null arguments given\n", __func__);
        return 1;
    }

    if ((fs_attr->id != HFS_FS_ATTR_ID_DATA) ||
        (fs_attr->type != TSK_FS_ATTR_TYPE_HFS_DATA)) {
        error_detected(TSK_ERR_FS_ARG,
            "%s: arg specified an attribute %u-%u that is not the data fork, "
            "Only the data fork can be compressed.", __func__, fs_attr->type,
            fs_attr->id);
        return 1;
    }

     
    if (!(fs_attr->flags & TSK_FS_ATTR_COMP)) {
        error_detected(TSK_ERR_FS_FWALK,
            "%s: called with non-special attribute: %x",
            __func__, fs_attr->flags);
        return 1;
    }

    fs = fs_attr->fs_file->fs_info;
    fs_file = fs_attr->fs_file;

     

    rAttr =
        tsk_fs_file_attr_get_type(fs_file, TSK_FS_ATTR_TYPE_HFS_RSRC,
        HFS_FS_ATTR_ID_RSRC, TRUE);
    if (rAttr == NULL) {
        error_returned
            (" %s: could not get the attribute for the resource fork of the file", __func__);
        return 1;
    }

    if (!read_block_table(rAttr, &offsetTable, &offsetTableSize, &offsetTableOffset)) {
      return 1;
    }

     
    rawBuf = (char *) tsk_malloc(COMPRESSION_UNIT_SIZE + 1);
    if (rawBuf == NULL) {
        error_returned
            (" %s: buffers for reading and uncompressing", __func__);
        goto on_error;
    }

    uncBuf = (char *) tsk_malloc(COMPRESSION_UNIT_SIZE);
    if (uncBuf == NULL) {
        error_returned
            (" %s: buffers for reading and uncompressing", __func__);
        goto on_error;
    }

    for (indx = 0; indx < offsetTableSize; ++indx) {
        ssize_t uncLen;         
        unsigned int blockSize;
        uint64_t lumpSize;
        uint64_t remaining;
        char *lumpStart;

        switch ((uncLen = read_and_decompress_block(
                    rAttr, rawBuf, uncBuf,
                    offsetTable, offsetTableSize, offsetTableOffset, indx,
                    decompress_block)))
        {
        case -1:
            goto on_error;
        case  0:
            continue;
        default:
            break;
        }

        blockSize = fs->block_size;
        remaining = uncLen;
        lumpStart = uncBuf;

        while (remaining > 0) {
            int retval;          
            lumpSize = remaining <= blockSize ? remaining : blockSize;

            if (tsk_verbose)
                tsk_fprintf(stderr,
                    "%s: Calling action on lump of size %"
                    PRIu64 " offset %" PRIu64 " in the compression unit\n",
                    __func__, lumpSize, uncLen - remaining);
            if (lumpSize > SIZE_MAX) {
                error_detected(TSK_ERR_FS_FWALK,
                    " %s: lumpSize is too large for the action", __func__);
                goto on_error;
            }

            retval = a_action(fs_attr->fs_file, off, 0, lumpStart,
                (size_t) lumpSize,    
                TSK_FS_BLOCK_FLAG_COMP, ptr);

            if (retval == TSK_WALK_ERROR) {
                error_detected(TSK_ERR_FS | 201,
                    "%s: callback returned an error", __func__);
                goto on_error;
            }
            else if (retval == TSK_WALK_STOP) {
                break;
            }

            off += lumpSize;
            remaining -= lumpSize;
            lumpStart += lumpSize;
        }
    }

    free(offsetTable);
    free(rawBuf);
    free(uncBuf);
    return 0;

on_error:
    free(offsetTable);
    free(rawBuf);
    free(uncBuf);
    return 0;
}
