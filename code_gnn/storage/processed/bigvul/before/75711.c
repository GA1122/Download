hfs_load_attrs(TSK_FS_FILE * fs_file)
{
    TSK_FS_INFO *fs;
    HFS_INFO *hfs;
    TSK_FS_ATTR *fs_attr;
    TSK_FS_ATTR_RUN *attr_run;
    hfs_fork *forkx;
    unsigned char resource_fork_has_contents = FALSE;
    unsigned char compression_flag = FALSE;
    unsigned char isCompressed = FALSE;
    unsigned char compDataInRSRCFork = FALSE;
    unsigned char cmpType = 0;
    uint64_t uncompressedSize;
    uint64_t logicalSize;        

    tsk_error_reset();

    if ((fs_file == NULL) || (fs_file->meta == NULL)
        || (fs_file->fs_info == NULL)) {
        error_detected(TSK_ERR_FS_ARG,
            "hfs_load_attrs: fs_file or meta is NULL");
        return 1;
    }

    fs = (TSK_FS_INFO *) fs_file->fs_info;
    hfs = (HFS_INFO *) fs;

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_load_attrs: Processing file %" PRIuINUM "\n",
            fs_file->meta->addr);


    if (fs_file->meta->attr_state == TSK_FS_META_ATTR_STUDIED) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_load_attrs: Attributes already loaded\n");
        return 0;
    }
    else if (fs_file->meta->attr_state == TSK_FS_META_ATTR_ERROR) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_load_attrs: Previous attempt to load attributes resulted in error\n");
        return 1;
    }

    if (fs_file->meta->attr != NULL) {
        tsk_fs_attrlist_markunused(fs_file->meta->attr);
    }
    else if (fs_file->meta->attr == NULL) {
        fs_file->meta->attr = tsk_fs_attrlist_alloc();
    }

     

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_load_attrs: loading the HFS+ extended attributes\n");

    if (hfs_load_extended_attrs(fs_file, &isCompressed,
            &cmpType, &uncompressedSize)) {
        error_returned(" - hfs_load_attrs A");
        fs_file->meta->attr_state = TSK_FS_META_ATTR_ERROR;
        return 1;
    }

    switch (cmpType) {
    case DECMPFS_TYPE_ZLIB_RSRC:
    case DECMPFS_TYPE_LZVN_RSRC:
        compDataInRSRCFork = TRUE;
        break;
    default:
        compDataInRSRCFork = FALSE;
        break;
    }

    if (isCompressed) {
        fs_file->meta->size = uncompressedSize;
    }

    compression_flag = (fs_file->meta->flags & TSK_FS_META_FLAG_COMP) != 0;

    if (compression_flag && !isCompressed) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_load_attrs: WARNING, HFS marks this as a"
                " compressed file, but no compression record was found.\n");
    }
    if (isCompressed && !compression_flag) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_load_attrs: WARNING, this file has a compression"
                " record, but the HFS compression flag is not set.\n");
    }

     

    if (fs_file->meta->content_ptr != NULL) {

         

        forkx = (hfs_fork *) fs_file->meta->content_ptr;

        if (!isCompressed) {
            logicalSize = tsk_getu64(fs->endian, forkx->logic_sz);

            if (logicalSize > 0 ||
                fs_file->meta->type == TSK_FS_META_TYPE_REG ||
                fs_file->meta->type == TSK_FS_META_TYPE_LNK) {


                if (tsk_verbose)
                    tsk_fprintf(stderr,
                        "hfs_load_attrs: loading the data fork attribute\n");

                if ((fs_attr = tsk_fs_attrlist_getnew(fs_file->meta->attr,
                            TSK_FS_ATTR_NONRES)) == NULL) {
                    error_returned(" - hfs_load_attrs");
                    return 1;
                }
                 

                if (logicalSize > 0) {

                    if (((attr_run =
                                hfs_extents_to_attr(fs, forkx->extents,
                                    0)) == NULL)
                        && (tsk_error_get_errno() != 0)) {
                        error_returned(" - hfs_load_attrs");
                        return 1;
                    }



                    if (tsk_fs_attr_set_run(fs_file, fs_attr, attr_run,
                            "", TSK_FS_ATTR_TYPE_HFS_DATA,
                            HFS_FS_ATTR_ID_DATA, logicalSize, logicalSize,
                            (TSK_OFF_T) tsk_getu32(fs->endian,
                                forkx->total_blk) * fs->block_size, 0,
                            0)) {
                        error_returned(" - hfs_load_attrs (DATA)");
                        tsk_fs_attr_run_free(attr_run);
                        return 1;
                    }

                    if (hfs_ext_find_extent_record_attr(hfs,
                            (uint32_t) fs_file->meta->addr, fs_attr,
                            TRUE)) {
                        error_returned(" - hfs_load_attrs B");
                        fs_file->meta->attr_state = TSK_FS_META_ATTR_ERROR;
                        return 1;
                    }

                }
                else {
                    if (tsk_fs_attr_set_run(fs_file, fs_attr, NULL, "",
                            TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA,
                            0, 0, 0, 0, 0)) {
                        error_returned(" - hfs_load_attrs (non-file)");
                        return 1;
                    }
                }

            }                    
        }                        



         


        forkx = &((hfs_fork *) fs_file->meta->content_ptr)[1];

        logicalSize = tsk_getu64(fs->endian, forkx->logic_sz);

        if (logicalSize > 0) {

            if (tsk_verbose)
                tsk_fprintf(stderr,
                    "hfs_load_attrs: loading the resource fork\n");

            resource_fork_has_contents = TRUE;

            if ((fs_attr =
                    tsk_fs_attrlist_getnew(fs_file->meta->attr,
                        TSK_FS_ATTR_NONRES)) == NULL) {
                error_returned(" - hfs_load_attrs (RSRC)");
                return 1;
            }
             


            if (((attr_run =
                        hfs_extents_to_attr(fs, forkx->extents,
                            0)) == NULL)
                && (tsk_error_get_errno() != 0)) {
                error_returned(" - hfs_load_attrs");
                return 1;
            }

            if (tsk_fs_attr_set_run(fs_file, fs_attr, attr_run, "RSRC",
                    TSK_FS_ATTR_TYPE_HFS_RSRC, HFS_FS_ATTR_ID_RSRC,
                    tsk_getu64(fs->endian, forkx->logic_sz),
                    tsk_getu64(fs->endian, forkx->logic_sz),
                    (TSK_OFF_T) tsk_getu32(fs->endian,
                        forkx->total_blk) * fs->block_size, 0, 0)) {
                error_returned(" - hfs_load_attrs (RSRC)");
                tsk_fs_attr_run_free(attr_run);
                return 1;
            }

            if (hfs_ext_find_extent_record_attr(hfs,
                    (uint32_t) fs_file->meta->addr, fs_attr, FALSE)) {
                error_returned(" - hfs_load_attrs C");
                fs_file->meta->attr_state = TSK_FS_META_ATTR_ERROR;
                return 1;
            }

            if (isCompressed && compDataInRSRCFork) {
                if (tsk_verbose)
                    tsk_fprintf(stderr,
                        "File is compressed with data in the resource fork. "
                        "Loading the default DATA attribute.\n");
                if ((fs_attr =
                        tsk_fs_attrlist_getnew(fs_file->meta->attr,
                            TSK_FS_ATTR_NONRES)) == NULL) {
                    error_returned
                        (" - hfs_load_attrs (RSRC loading as DATA)");
                    return 1;
                }
                 

                switch (cmpType) {
                case DECMPFS_TYPE_ZLIB_RSRC:
#ifdef HAVE_LIBZ
                    fs_attr->w = hfs_attr_walk_zlib_rsrc;
                    fs_attr->r = hfs_file_read_zlib_rsrc;
#else
                    if (tsk_verbose)
                        tsk_fprintf(stderr,
                            "hfs_load_attrs: No zlib compression library, so setting a zero-length default DATA attribute.\n");

                    if (tsk_fs_attr_set_run(fs_file, fs_attr, NULL, "DATA",
                            TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA, 0,
                            0, 0, 0, 0)) {
                        error_returned(" - hfs_load_attrs (non-file)");
                        return 1;
                    }
#endif
                    break;

                case DECMPFS_TYPE_LZVN_RSRC:

                    fs_attr->w = hfs_attr_walk_lzvn_rsrc;
                    fs_attr->r = hfs_file_read_lzvn_rsrc;

                    break;
                }

                if (((attr_run =
                            hfs_extents_to_attr(fs, forkx->extents,
                                0)) == NULL)
                    && (tsk_error_get_errno() != 0)) {
                    error_returned
                        (" - hfs_load_attrs, RSRC fork as DATA fork");
                    return 1;
                }

                if (tsk_verbose)
                    tsk_fprintf(stderr,
                        "hfs_load_attrs:  Loading RSRC fork block runs as the default DATA attribute.\n");

                if (tsk_fs_attr_set_run(fs_file, fs_attr, attr_run, "DECOMP",
                        TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA,
                        logicalSize,
                        logicalSize,
                        (TSK_OFF_T) tsk_getu32(fs->endian,
                            forkx->total_blk) * fs->block_size,
                        TSK_FS_ATTR_COMP | TSK_FS_ATTR_NONRES, 0)) {
                    error_returned
                        (" - hfs_load_attrs (RSRC loading as DATA)");
                    tsk_fs_attr_run_free(attr_run);
                    return 1;
                }

                if (hfs_ext_find_extent_record_attr(hfs,
                        (uint32_t) fs_file->meta->addr, fs_attr, FALSE)) {
                    error_returned
                        (" - hfs_load_attrs (RSRC loading as DATA");
                    fs_file->meta->attr_state = TSK_FS_META_ATTR_ERROR;
                    return 1;
                }

                if (tsk_verbose)
                    tsk_fprintf(stderr,
                        "hfs_load_attrs: setting the \"special\" function pointers to inflate compressed data.\n");
            }

        }                        

    }                            

    if (isCompressed && compDataInRSRCFork && !resource_fork_has_contents) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_load_attrs: WARNING, compression record claims that compressed data"
                " is in the Resource Fork, but that fork is empty or non-existent.\n");
    }

    fs_file->meta->attr_state = TSK_FS_META_ATTR_STUDIED;

    return 0;
}
