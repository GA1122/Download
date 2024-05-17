exsltCryptoGcryptInit (void) {
    static int gcrypt_init;
    xmlLockLibrary ();

    if (!gcrypt_init) {
 
	gcry_check_version (GCRYPT_VERSION);
	gcrypt_init = 1;
    }

    xmlUnlockLibrary ();
}
