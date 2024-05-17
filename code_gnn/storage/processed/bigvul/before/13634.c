static int load_CA(SSL_CTX *ctx, char *file)
{
    FILE *in;
    X509 *x = NULL;

    if ((in = fopen(file, "r")) == NULL)
        return (0);

    for (;;) {
        if (PEM_read_X509(in, &x, NULL) == NULL)
            break;
        SSL_CTX_add_client_CA(ctx, x);
    }
    if (x != NULL)
        X509_free(x);
    fclose(in);
    return (1);
}
