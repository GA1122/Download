WhoIsThis(struct rx_call *acall, struct ubik_trans *at, afs_int32 *aid)
{
    int foreign = 0;
     
     
    struct rx_connection *tconn;
    afs_int32 code;
    char tcell[MAXKTCREALMLEN];
    char name[MAXKTCNAMELEN];
    char inst[MAXKTCNAMELEN];
    int ilen;
    char vname[256];

    *aid = ANONYMOUSID;
    tconn = rx_ConnectionOf(acall);
    code = rx_SecurityClassOf(tconn);
    if (code == 0)
	return 0;
    else if (code == 1) {	 
	goto done;		 
    } else if (code == 2) {	 
	if ((code = rxkad_GetServerInfo(acall->conn, NULL, 0   ,
					name, inst, tcell, NULL)))
	    goto done;
#if 0
	 
	if (exp < FT_ApproxTime())
	    goto done;
#endif
	if (tcell[0])
	    foreign = afs_is_foreign_ticket_name(name,inst,tcell,pr_realmName);

	strncpy(vname, name, sizeof(vname));
	if ((ilen = strlen(inst))) {
	    if (strlen(vname) + 1 + ilen >= sizeof(vname))
		goto done;
	    strcat(vname, ".");
	    strcat(vname, inst);
	}
	if (foreign) {
	    if (strlen(vname) + strlen(tcell) + 1 >= sizeof(vname))
		goto done;
	    strcat(vname, "@");
	    strcat(vname, tcell);
	}
	if (strcmp(AUTH_SUPERUSER, vname) == 0)
	    *aid = SYSADMINID;	 
	else {
	    lcstring(vname, vname, sizeof(vname));
	    code = NameToID(at, vname, aid);
	}
    }
  done:
    if (code && !pr_noAuth)
	return -1;
    return 0;
}
