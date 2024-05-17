int remove_directory(const char *path) {
	return nftw(path, remove_callback, 64, FTW_DEPTH | FTW_PHYS);
}
