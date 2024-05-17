char * SafeSock::serialize(char *buf)
{
	char sinful_string[28];
	char usernamebuf[128];
	char *ptmp, *ptr = NULL;
    
	ASSERT(buf);
    memset(sinful_string, 0, 28);
    memset(usernamebuf, 0, 128);

	ptmp = Sock::serialize(buf);
	ASSERT( ptmp );
	int itmp;
	sscanf(ptmp,"%d*",&itmp);
	_special_state=safesock_state(itmp);
    ptmp = strchr(ptmp, '*');
    if(ptmp) ptmp++;

    if (ptmp && (ptr = strchr(ptmp, '*')) != NULL) {
        memcpy(sinful_string, ptmp, ptr - ptmp);
        ptmp = ++ptr;
    }
    else if(ptmp) {
        sscanf(ptmp,"%s",sinful_string);
    }

	_who.from_sinful(sinful_string);

	return NULL;
}
