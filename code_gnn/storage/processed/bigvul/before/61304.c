EXPORTED int mboxlist_usermboxtree(const char *userid, mboxlist_cb *proc,
                                   void *rock, int flags)
{
    char *inbox = mboxname_user_mbox(userid, 0);
    int r = mboxlist_mboxtree(inbox, proc, rock, flags);

    if (flags & MBOXTREE_PLUS_RACL) {
        struct allmb_rock mbrock = { NULL, flags, proc, rock };
         
        struct buf buf = BUF_INITIALIZER;
        strarray_t matches = STRARRAY_INITIALIZER;

         
        mboxlist_racl_key(1, userid, NULL, &buf);
         
        struct raclrock raclrock = { buf.len, &matches };
         
        r = cyrusdb_foreach(mbdb, buf.s, buf.len, NULL, racl_cb, &raclrock, NULL);
        buf_reset(&buf);

         
        mboxlist_racl_key(0, userid, NULL, &buf);
        raclrock.prefixlen = buf.len;
        if (!r) r = cyrusdb_foreach(mbdb, buf.s, buf.len, NULL, racl_cb, &raclrock, NULL);

         
        int i;
        for (i = 0; !r && i < strarray_size(&matches); i++) {
            const char *mboxname = strarray_nth(&matches, i);
            r = cyrusdb_forone(mbdb, mboxname, strlen(mboxname), allmbox_p, allmbox_cb, &mbrock, 0);
        }
        buf_free(&buf);
        strarray_fini(&matches);
        mboxlist_entry_free(&mbrock.mbentry);
    }

    free(inbox);
    return r;
}
