FileTransfer::AddDownloadFilenameRemaps(char const *remaps) {
	if(!download_filename_remaps.IsEmpty()) {
		download_filename_remaps += ";";
	}
	download_filename_remaps += remaps;
}
