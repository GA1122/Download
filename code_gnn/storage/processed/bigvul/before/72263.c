try_identity(Identity *id)
{
	if (!id->key)
		return (0);
	if (key_type_plain(id->key->type) == KEY_RSA &&
	    (datafellows & SSH_BUG_RSASIGMD5) != 0) {
		debug("Skipped %s key %s for RSA/MD5 server",
		    key_type(id->key), id->filename);
		return (0);
	}
	return (id->key->type != KEY_RSA1);
}
