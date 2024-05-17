FileTransfer::InitDownloadFilenameRemaps(ClassAd *Ad) {
	char *remap_fname = NULL;

	dprintf(D_FULLDEBUG,"Entering FileTransfer::InitDownloadFilenameRemaps\n");

	download_filename_remaps = "";
	if(!Ad) return 1;

	if (Ad->LookupString(ATTR_TRANSFER_OUTPUT_REMAPS,&remap_fname)) {
		AddDownloadFilenameRemaps(remap_fname);
		free(remap_fname);
		remap_fname = NULL;
	}

	if(!download_filename_remaps.IsEmpty()) {
		dprintf(D_FULLDEBUG, "FileTransfer: output file remaps: %s\n",download_filename_remaps.Value());
	}
	return 1;
}
