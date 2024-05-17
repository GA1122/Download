const EVP_CIPHER *EVP_aes_128_cbc_hmac_sha256(void)
{
    return ((OPENSSL_ia32cap_P[1] & AESNI_CAPABLE) &&
            aesni_cbc_sha256_enc(NULL, NULL, 0, NULL, NULL, NULL, NULL) ?
            &aesni_128_cbc_hmac_sha256_cipher : NULL);
}
