static void authentication_success(void)
{
    int r;
    struct mboxevent *mboxevent;

     
    imapd_userisadmin = global_authisa(imapd_authstate, IMAPOPT_ADMINS);

     
    imapd_logfd = telemetry_log(imapd_userid, imapd_in, imapd_out, 0);

     
    r = mboxname_init_namespace(&imapd_namespace,
                                imapd_userisadmin || imapd_userisproxyadmin);

    mboxevent_setnamespace(&imapd_namespace);

    if (r) {
        syslog(LOG_ERR, "%s", error_message(r));
        fatal(error_message(r), EC_CONFIG);
    }

     
    proxy_userid = xstrdup(imapd_userid);

     
    if ((mboxevent = mboxevent_new(EVENT_LOGIN))) {
        mboxevent_set_access(mboxevent, saslprops.iplocalport,
                             saslprops.ipremoteport, imapd_userid, NULL, 1);

        mboxevent_notify(&mboxevent);
        mboxevent_free(&mboxevent);
    }

#ifdef USE_AUTOCREATE
    autocreate_inbox();
#endif  
}
