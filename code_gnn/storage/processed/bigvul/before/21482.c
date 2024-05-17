static kadm5_ret_t add_to_history(krb5_context context,
                                  krb5_kvno hist_kvno,
                                  osa_princ_ent_t adb,
                                  kadm5_policy_ent_t pol,
                                  osa_pw_hist_ent *pw)
{
    osa_pw_hist_ent *histp;
    uint32_t nhist;
    unsigned int i, knext, nkeys;

    nhist = pol->pw_history_num;
     
    if (nhist <= 1)
        return 0;

    if (adb->admin_history_kvno != hist_kvno) {
         
        free(adb->old_keys);
        adb->old_keys = NULL;
        adb->old_key_len = 0;
        adb->old_key_next = 0;
        adb->admin_history_kvno = hist_kvno;
    }

    nkeys = adb->old_key_len;
    knext = adb->old_key_next;
     
    if (nkeys + 1 < nhist) {
        if (adb->old_keys == NULL) {
            adb->old_keys = (osa_pw_hist_ent *)
                malloc((nkeys + 1) * sizeof (osa_pw_hist_ent));
        } else {
            adb->old_keys = (osa_pw_hist_ent *)
                realloc(adb->old_keys,
                        (nkeys + 1) * sizeof (osa_pw_hist_ent));
        }
        if (adb->old_keys == NULL)
            return(ENOMEM);

        memset(&adb->old_keys[nkeys], 0, sizeof(osa_pw_hist_ent));
        nkeys = ++adb->old_key_len;
         
        for (i = nkeys - 1; i > knext; i--) {
            adb->old_keys[i] = adb->old_keys[i - 1];
        }
        memset(&adb->old_keys[knext], 0, sizeof(osa_pw_hist_ent));
    } else if (nkeys + 1 > nhist) {
         
         
        int j;
        osa_pw_hist_t tmp;

        tmp = (osa_pw_hist_ent *)
            malloc((nhist - 1) * sizeof (osa_pw_hist_ent));
        if (tmp == NULL)
            return ENOMEM;
        for (i = 0; i < nhist - 1; i++) {
             
            j = (i + nkeys + knext - (nhist - 1)) % nkeys;
            tmp[i] = adb->old_keys[j];
        }
         
        for (i = 0; i < nkeys - (nhist - 1); i++) {
            j = (i + nkeys + knext) % nkeys;
            histp = &adb->old_keys[j];
            for (j = 0; j < histp->n_key_data; j++) {
                krb5_free_key_data_contents(context, &histp->key_data[j]);
            }
            free(histp->key_data);
        }
        free(adb->old_keys);
        adb->old_keys = tmp;
        nkeys = adb->old_key_len = nhist - 1;
        knext = adb->old_key_next = 0;
    }

     
    if (knext + 1 > nkeys)
        knext = adb->old_key_next = 0;
     
    histp = &adb->old_keys[knext];
    for (i = 0; i < (unsigned int) histp->n_key_data; i++)
        krb5_free_key_data_contents(context, &histp->key_data[i]);
    free(histp->key_data);

     
    adb->old_keys[knext] = *pw;

     
    if (++adb->old_key_next == nhist - 1)
        adb->old_key_next = 0;

    return(0);
}
