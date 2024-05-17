pgp_new_blob(sc_card_t *card, pgp_blob_t *parent, unsigned int file_id,
		sc_file_t *file)
{
	pgp_blob_t *blob = NULL;

	if (file == NULL)
		return NULL;

	if ((blob = calloc(1, sizeof(pgp_blob_t))) != NULL) {
		struct pgp_priv_data *priv = DRVDATA(card);
		struct do_info *info;

		blob->file = file;

		blob->file->type         = SC_FILE_TYPE_WORKING_EF;  
		blob->file->ef_structure = SC_FILE_EF_TRANSPARENT;
		blob->file->id           = file_id;

		blob->id     = file_id;
		blob->parent = parent;

		if (parent != NULL) {
			pgp_blob_t **p;

			 
			blob->file->path = parent->file->path;
			sc_append_file_id(&blob->file->path, file_id);

			 
			for (p = &parent->files; *p != NULL; p = &(*p)->next)
				;
			*p = blob;
		}
		else {
			char path[10] = "0000";	 

			 
			if (4 != snprintf(path, sizeof(path), "%04X", file_id & 0xFFFF)) {
				free(blob);
				return NULL;
			}

			sc_format_path(path, &blob->file->path);
		}

		 
		for (info = priv->pgp_objects; (info != NULL) && (info->id > 0); info++) {
			if (info->id == file_id) {
				blob->info = info;
				blob->file->type = blob->info->type;
				pgp_attach_acl(card, blob->file, info);
				break;
			}
		}
	}

	return blob;
}
