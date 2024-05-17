static int add_recent_loose(const unsigned char *sha1,
			    const char *path, void *data)
{
	struct stat st;
	struct object *obj = lookup_object(sha1);

	if (obj && obj->flags & SEEN)
		return 0;

	if (stat(path, &st) < 0) {
		 
		if (errno == ENOENT)
			return 0;
		return error("unable to stat %s: %s",
			     sha1_to_hex(sha1), strerror(errno));
	}

	add_recent_object(sha1, st.st_mtime, data);
	return 0;
}
