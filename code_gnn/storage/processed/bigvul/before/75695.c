hfs_file_read_compressed_rsrc(const TSK_FS_ATTR * a_fs_attr,
    TSK_OFF_T a_offset, char *a_buf, size_t a_len,
    int (*read_block_table)(const TSK_FS_ATTR *rAttr,
                            CMP_OFFSET_ENTRY** offsetTableOut,
                            uint32_t* tableSizeOut,
                            uint32_t* tableOffsetOut),
    int (*decompress_block)(char* rawBuf,
                            uint32_t len,
                            char* uncBuf,
                            uint64_t* uncLen))
{
    TSK_FS_FILE *fs_file;
    const TSK_FS_ATTR *rAttr;
    char *rawBuf = NULL;
    char *uncBuf = NULL;
    uint32_t offsetTableOffset;
    uint32_t offsetTableSize;          
    CMP_OFFSET_ENTRY *offsetTable = NULL;
    TSK_OFF_T indx;                 
    TSK_OFF_T startUnit = 0;
    uint32_t startUnitOffset = 0;
    TSK_OFF_T endUnit = 0;
    uint64_t bytesCopied;

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "%s: called because this file is compressed, with data in the resource fork\n", __func__);

    if (a_len == 0)
        return 0;

    if (a_offset < 0) {
        error_detected(TSK_ERR_FS_ARG,
            "%s: reading from file at a negative offset",
             __func__);
        return -1;
    }

    if (a_len > SIZE_MAX / 2) {
        error_detected(TSK_ERR_FS_ARG,
            "%s: trying to read more than SIZE_MAX/2 is not supported.",
            __func__);
        return -1;
    }

    if ((a_fs_attr == NULL) || (a_fs_attr->fs_file == NULL)
        || (a_fs_attr->fs_file->meta == NULL)
        || (a_fs_attr->fs_file->fs_info == NULL)) {
        error_detected(TSK_ERR_FS_ARG,
            "%s: NULL parameters passed", __func__);
        return -1;
    }

    if (!(a_fs_attr->flags & TSK_FS_ATTR_COMP)) {
        error_detected(TSK_ERR_FS_ARG,
            "%s: called with non-special attribute: %x",
            __func__, a_fs_attr->flags);
        return -1;
    }

    if ((a_fs_attr->id != HFS_FS_ATTR_ID_DATA) ||
        (a_fs_attr->type != TSK_FS_ATTR_TYPE_HFS_DATA)) {
        error_detected(TSK_ERR_FS_ARG,
            "%s: arg specified an attribute %u-%u that is not the data fork, "
            "Only the data fork can be compressed.", __func__,
            a_fs_attr->type, a_fs_attr->id);
        return -1;
    }

     
    fs_file = a_fs_attr->fs_file;

    rAttr =
        tsk_fs_file_attr_get_type(fs_file, TSK_FS_ATTR_TYPE_HFS_RSRC,
        HFS_FS_ATTR_ID_RSRC, TRUE);
    if (rAttr == NULL) {
        error_returned
            (" %s: could not get the attribute for the resource fork of the file", __func__);
        return -1;
    }

    if (!read_block_table(rAttr, &offsetTable, &offsetTableSize, &offsetTableOffset)) {
      return -1;
    }

    startUnit = a_offset / COMPRESSION_UNIT_SIZE;
    startUnitOffset = a_offset % COMPRESSION_UNIT_SIZE;
    endUnit = (a_offset + a_len - 1) / COMPRESSION_UNIT_SIZE;

    if (startUnit >= offsetTableSize || endUnit >= offsetTableSize) {
        error_detected(TSK_ERR_FS_ARG,
            "%s: range of bytes requested %lld - %lld falls past the "
            "end of the uncompressed stream %llu\n",
            __func__, a_offset, a_offset + a_len,
            offsetTable[offsetTableSize-1].offset +
            offsetTable[offsetTableSize-1].length);
        goto on_error;
    }

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "%s: reading compression units: %" PRIuOFF
            " to %" PRIuOFF "\n", __func__, startUnit, endUnit);
    bytesCopied = 0;

     
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

    for (indx = startUnit; indx <= endUnit; ++indx) {
        uint64_t uncLen;
        char *uncBufPtr = uncBuf;
        size_t bytesToCopy;

        switch ((uncLen = read_and_decompress_block(
                    rAttr, rawBuf, uncBuf,
                    offsetTable, offsetTableSize, offsetTableOffset, (size_t)indx,
                    decompress_block)))
        {
        case -1:
            goto on_error;
        case  0:
            continue;
        default:
            break;
        }

        if (indx == startUnit) {
            uncLen -= startUnitOffset;
            uncBufPtr += startUnitOffset;
        }


        if (bytesCopied + uncLen < (uint64_t) a_len)     
            bytesToCopy = (size_t) uncLen;       
        else
            bytesToCopy = (size_t) (((uint64_t) a_len) - bytesCopied);   

        memcpy(a_buf + bytesCopied, uncBufPtr, bytesToCopy);
        bytesCopied += bytesToCopy;
    }


    if (bytesCopied < a_len) {
        memset(a_buf + bytesCopied, 0, a_len - (size_t) bytesCopied);    
    }

    free(offsetTable);
    free(rawBuf);
    free(uncBuf);

    return (ssize_t) bytesCopied;        

on_error:
    free(offsetTable);
    free(rawBuf);
    free(uncBuf);
    return -1;
}
