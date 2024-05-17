GSF_CLASS (GsfInfileTar, gsf_infile_tar,
	   gsf_infile_tar_class_init, gsf_infile_tar_init,
	   GSF_INFILE_TYPE)

 
GsfInfile *
gsf_infile_tar_new (GsfInput *source, GError **err)
{
	GsfInfileTar *tar;

	g_return_val_if_fail (GSF_IS_INPUT (source), NULL);

	tar = g_object_new (GSF_INFILE_TAR_TYPE,
			    "source", source,
			    NULL);

	if (tar->err) {
		if (err)
			*err = g_error_copy (tar->err);
		g_object_unref (tar);
		return NULL;
	}

	return GSF_INFILE (tar);
}