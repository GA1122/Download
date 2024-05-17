static void list_data_remotesubscriptions(struct listargs *listargs)
{
     
    struct list_rock rock;
    char mytag[128];

    memset(&rock, 0, sizeof(struct list_rock));
    rock.listargs = listargs;
    rock.subs = strarray_new();
    construct_hash_table(&rock.server_table, 10, 1);

    proxy_gentag(mytag, sizeof(mytag));

    if ((listargs->sel & LIST_SEL_SUBSCRIBED) &&
        !(listargs->sel & (LIST_SEL_SPECIALUSE | LIST_SEL_METADATA))) {
         
        list_data_remote(backend_inbox, mytag, listargs, rock.subs);

         
        hash_insert(backend_inbox->hostname,
                    (void *)0xDEADBEEF, &rock.server_table);
    }
    else {
         
        struct listargs myargs;

        memcpy(&myargs, listargs, sizeof(struct listargs));
        myargs.sel = LIST_SEL_SUBSCRIBED;
        myargs.ret = 0;

        list_data_remote(backend_inbox, mytag, &myargs, rock.subs);
    }

     
    mboxlist_findallmulti(&imapd_namespace, &listargs->pat,
                          imapd_userisadmin, imapd_userid,
                          imapd_authstate, list_cb, &rock);

    strarray_free(rock.subs);
    free_hash_table(&rock.server_table, NULL);
    if (rock.last_name) free(rock.last_name);
}
