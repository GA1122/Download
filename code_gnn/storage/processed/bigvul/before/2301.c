_PUBLIC_ char *strlower_talloc_handle(struct smb_iconv_handle *iconv_handle,
				      TALLOC_CTX *ctx, const char *src)
{
	size_t size=0;
	char *dest;

	if(src == NULL) {
		return NULL;
	}

	 
	dest = talloc_array(ctx, char, 2*(strlen(src))+1);
	if (dest == NULL) {
		return NULL;
	}

	while (*src) {
		size_t c_size;
		codepoint_t c = next_codepoint_handle(iconv_handle, src, &c_size);
		src += c_size;

		c = tolower_m(c);

		c_size = push_codepoint_handle(iconv_handle, dest+size, c);
		if (c_size == -1) {
			talloc_free(dest);
			return NULL;
		}
		size += c_size;
	}

	dest[size] = 0;

	 
	dest = talloc_realloc(ctx, dest, char, size+1);

	talloc_set_name_const(dest, dest);

	return dest;
}
