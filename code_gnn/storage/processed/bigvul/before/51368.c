PHP_FUNCTION(mcrypt_enc_get_algorithms_name)
{
	char *name;
	MCRYPT_GET_TD_ARG

	name = mcrypt_enc_get_algorithms_name(pm->td);
	RETVAL_STRING(name, 1);
	mcrypt_free(name);
}
