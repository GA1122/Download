free_certauth_handles(krb5_context context, certauth_handle *list)
{
    int i;

    if (list == NULL)
        return;
    for (i = 0; list[i] != NULL; i++) {
        if (list[i]->vt.fini != NULL)
            list[i]->vt.fini(context, list[i]->moddata);
        free(list[i]);
    }
    free(list);
}
