static void php_zlib_free(voidpf opaque, voidpf address)
{
	efree((void*)address);
}
