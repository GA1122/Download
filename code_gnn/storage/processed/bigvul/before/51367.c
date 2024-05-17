PHP_FUNCTION(mcrypt_enc_get_iv_size)
{
	MCRYPT_GET_TD_ARG
	RETURN_LONG(mcrypt_enc_get_iv_size(pm->td));
}
