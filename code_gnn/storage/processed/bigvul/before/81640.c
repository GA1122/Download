construct_etag(char *buf, size_t buf_len, const struct mg_file_stat *filestat)
{
	if ((filestat != NULL) && (buf != NULL)) {
		mg_snprintf(NULL,
		            NULL,  
		            buf,
		            buf_len,
		            "\"%lx.%" INT64_FMT "\"",
		            (unsigned long)filestat->last_modified,
		            filestat->size);
	}
}