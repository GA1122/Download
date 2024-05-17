PHP_FUNCTION(mcrypt_enc_get_block_size)
{
	MCRYPT_GET_TD_ARG
	RETURN_LONG(mcrypt_enc_get_block_size(pm->td));
}
