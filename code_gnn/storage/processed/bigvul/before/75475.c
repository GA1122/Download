kdc_get_ticket_endtime(kdc_realm_t *kdc_active_realm,
                       krb5_timestamp starttime,
                       krb5_timestamp endtime,
                       krb5_timestamp till,
                       krb5_db_entry *client,
                       krb5_db_entry *server,
                       krb5_timestamp *out_endtime)
{
    krb5_timestamp until;
    krb5_deltat life;

    if (till == 0)
        till = kdc_infinity;

    until = ts_min(till, endtime);

     
    life = ts_delta(until, starttime);
    if (ts_after(until, starttime) && life < 0)
        life = INT32_MAX;

    if (client != NULL && client->max_life != 0)
        life = min(life, client->max_life);
    if (server->max_life != 0)
        life = min(life, server->max_life);
    if (kdc_active_realm->realm_maxlife != 0)
        life = min(life, kdc_active_realm->realm_maxlife);

    *out_endtime = ts_incr(starttime, life);
}