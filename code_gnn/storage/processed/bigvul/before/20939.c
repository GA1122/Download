cdf_read_short_stream(const cdf_info_t *info, const cdf_header_t *h,
    const cdf_sat_t *sat, const cdf_dir_t *dir, cdf_stream_t *scn)
{
	size_t i;
	const cdf_directory_t *d;

	for (i = 0; i < dir->dir_len; i++)
		if (dir->dir_tab[i].d_type == CDF_DIR_TYPE_ROOT_STORAGE)
			break;

	 
	if (i == dir->dir_len)
		goto out;
	d = &dir->dir_tab[i];

	 
	if (d->d_stream_first_sector < 0)
		goto out;

	return	cdf_read_long_sector_chain(info, h, sat,
	    d->d_stream_first_sector, d->d_size, scn);
out:
	scn->sst_tab = NULL;
	scn->sst_len = 0;
	scn->sst_dirlen = 0;
	return 0;
}