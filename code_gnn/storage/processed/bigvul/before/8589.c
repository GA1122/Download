static void ssh_send_verstring(Ssh ssh, const char *protoname, char *svers)
{
    char *verstring;

    if (ssh->version == 2) {
	 
	verstring = dupprintf("%s2.0-%s\015\012", protoname, sshver);
    } else {
	 
        assert(!strcmp(protoname, "SSH-"));  
	verstring = dupprintf("SSH-%s-%s\012",
			      (ssh_versioncmp(svers, "1.5") <= 0 ?
			       svers : "1.5"),
			      sshver);
    }

    ssh_fix_verstring(verstring + strlen(protoname));
#ifdef FUZZING
     
    verstring[0] = 'I';
#endif

    if (ssh->version == 2) {
	size_t len;
	 
	len = strcspn(verstring, "\015\012");
	ssh->v_c = snewn(len + 1, char);
	memcpy(ssh->v_c, verstring, len);
	ssh->v_c[len] = 0;
    }

    logeventf(ssh, "We claim version: %.*s",
	      strcspn(verstring, "\015\012"), verstring);
    s_write(ssh, verstring, strlen(verstring));
    sfree(verstring);
}