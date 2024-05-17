ossl_cipher_set_iv_length(VALUE self, VALUE iv_length)
{
    int len = NUM2INT(iv_length);
    EVP_CIPHER_CTX *ctx;

    GetCipher(self, ctx);
    if (!(EVP_CIPHER_CTX_flags(ctx) & EVP_CIPH_FLAG_AEAD_CIPHER))
	ossl_raise(eCipherError, "cipher does not support AEAD");

    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, len, NULL))
	ossl_raise(eCipherError, "unable to set IV length");

     
    EVP_CIPHER_CTX_set_app_data(ctx, (void *)(VALUE)len);

    return iv_length;
}