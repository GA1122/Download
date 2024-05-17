static void wc_ecc_free_async(ecc_key* key)
{
    wc_ecc_free_mpint(key, &key->r);
    wc_ecc_free_mpint(key, &key->s);
#ifdef HAVE_CAVIUM_V
    wc_ecc_free_mpint(key, &key->e);
    wc_ecc_free_mpint(key, &key->signK);
#endif  
}