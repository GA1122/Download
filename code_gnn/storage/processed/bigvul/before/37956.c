apprentice_load(struct magic_set *ms, const char *fn, int action)
{
	int errs = 0;
	uint32_t i, j;
	size_t files = 0, maxfiles = 0;
	char **filearr = NULL, *mfn;
	struct stat st;
	struct magic_map *map;
	struct magic_entry_set mset[MAGIC_SETS];
	DIR *dir;
	struct dirent *d;

	memset(mset, 0, sizeof(mset));
	ms->flags |= MAGIC_CHECK;	 


	if ((map = CAST(struct magic_map *, calloc(1, sizeof(*map)))) == NULL)
	{
		file_oomem(ms, sizeof(*map));
		return NULL;
	}

	 
	if (action == FILE_CHECK)
		(void)fprintf(stderr, "%s\n", usg_hdr);

	 
	if (stat(fn, &st) == 0 && S_ISDIR(st.st_mode)) {
		dir = opendir(fn);
		if (!dir) {
			errs++;
			goto out;
		}
		while ((d = readdir(dir)) != NULL) {
			if (asprintf(&mfn, "%s/%s", fn, d->d_name) < 0) {
				file_oomem(ms,
				    strlen(fn) + strlen(d->d_name) + 2);
				errs++;
				closedir(dir);
				goto out;
			}
			if (stat(mfn, &st) == -1 || !S_ISREG(st.st_mode)) {
				free(mfn);
				continue;
			}
			if (files >= maxfiles) {
				size_t mlen;
				maxfiles = (maxfiles + 1) * 2;
				mlen = maxfiles * sizeof(*filearr);
				if ((filearr = CAST(char **,
				    realloc(filearr, mlen))) == NULL) {
					file_oomem(ms, mlen);
					free(mfn);
					closedir(dir);
					errs++;
					goto out;
				}
			}
			filearr[files++] = mfn;
		}
		closedir(dir);
		qsort(filearr, files, sizeof(*filearr), cmpstrp);
		for (i = 0; i < files; i++) {
			load_1(ms, action, filearr[i], &errs, mset);
			free(filearr[i]);
		}
		free(filearr);
	} else
		load_1(ms, action, fn, &errs, mset);
	if (errs)
		goto out;

	for (j = 0; j < MAGIC_SETS; j++) {
		 
		for (i = 0; i < mset[j].count; ) {
			if (mset[j].me[i].mp->cont_level != 0) {
				i++;
				continue;
			}
			i = set_text_binary(ms, mset[j].me, mset[j].count, i);
		}
		qsort(mset[j].me, mset[j].count, sizeof(*mset[j].me),
		    apprentice_sort);

		 
		set_last_default(ms, mset[j].me, mset[j].count);

		 
		if (coalesce_entries(ms, mset[j].me, mset[j].count,
		    &map->magic[j], &map->nmagic[j]) == -1) {
			errs++;
			goto out;
		}
	}

out:
	for (j = 0; j < MAGIC_SETS; j++)
		magic_entry_free(mset[j].me, mset[j].count);

	if (errs) {
		apprentice_unmap(map);
		return NULL;
	}
	return map;
}