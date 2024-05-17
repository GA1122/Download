static int mboxlist_find_category(struct find_rock *rock, const char *prefix, size_t len)
{
    int r = 0;
    if (!rock->issubs && !rock->isadmin && !cyrusdb_fetch(rock->db, "$RACL", 5, NULL, NULL, NULL)) {
         
        struct buf buf = BUF_INITIALIZER;
        strarray_t matches = STRARRAY_INITIALIZER;
        mboxlist_racl_key(rock->mb_category == MBNAME_OTHERUSER, rock->userid, NULL, &buf);
         
        struct raclrock raclrock = { buf.len, &matches };
         
        if (len) buf_appendmap(&buf, prefix, len);
        r = cyrusdb_foreach(rock->db, buf.s, buf.len, NULL, racl_cb, &raclrock, NULL);
         
        int i;
        for (i = 0; !r && i < strarray_size(&matches); i++) {
            const char *key = strarray_nth(&matches, i);
            r = cyrusdb_forone(rock->db, key, strlen(key), &find_p, &find_cb, rock, NULL);
        }
        strarray_fini(&matches);
    }
    else {
        r = cyrusdb_foreach(rock->db, prefix, len, &find_p, &find_cb, rock, NULL);
    }

    if (r == CYRUSDB_DONE) r = 0;
    return r;
}
