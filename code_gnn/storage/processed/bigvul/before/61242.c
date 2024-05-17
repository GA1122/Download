static int find_p(void *rockp,
                  const char *key, size_t keylen,
                  const char *data, size_t datalen)
{
    struct find_rock *rock = (struct find_rock *) rockp;
    char intname[MAX_MAILBOX_PATH+1];
    int i;

     
    if (key[0] == '$') return 0;

    memcpy(intname, key, keylen);
    intname[keylen] = 0;

    assert(!rock->mbname);
    rock->mbname = mbname_from_intname(intname);

    if (!rock->isadmin && !config_getswitch(IMAPOPT_CROSSDOMAINS)) {
         
        if (strcmpsafe(rock->domain, mbname_domain(rock->mbname)))
            goto nomatch;
    }

    if (rock->mb_category && mbname_category(rock->mbname, rock->namespace, rock->userid) != rock->mb_category)
        goto nomatch;

     
    const char *extname = mbname_extname(rock->mbname, rock->namespace, rock->userid);
    if (!extname) goto nomatch;

    int matchlen = 0;
    for (i = 0; i < rock->globs.count; i++) {
        glob *g = ptrarray_nth(&rock->globs, i);
        int thismatch = glob_test(g, extname);
        if (thismatch > matchlen) matchlen = thismatch;
    }

     
    if (!matchlen) goto nomatch;

    rock->matchlen = matchlen;

     
    if (rock->issubs)
        goto good;

     
    if (mboxlist_parse_entry(&rock->mbentry, key, keylen, data, datalen))
        goto nomatch;

     
    if (rock->mbentry->mbtype & MBTYPE_DELETED)
        goto nomatch;

     
    if (!rock->isadmin) {
         
        if (mbname_isdeleted(rock->mbname)) goto nomatch;

         
        if (!(cyrus_acl_myrights(rock->auth_state, rock->mbentry->acl) & ACL_LOOKUP)) goto nomatch;
    }

good:
    return 1;

nomatch:
    mboxlist_entry_free(&rock->mbentry);
    mbname_free(&rock->mbname);
    return 0;
}
