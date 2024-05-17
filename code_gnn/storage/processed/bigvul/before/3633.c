static int rsa_md_to_algor(X509_ALGOR **palg, const EVP_MD *md)
{
    if (EVP_MD_type(md) == NID_sha1)
        return 1;
    *palg = X509_ALGOR_new();
    if (!*palg)
        return 0;
    X509_ALGOR_set_md(*palg, md);
    return 1;
}
