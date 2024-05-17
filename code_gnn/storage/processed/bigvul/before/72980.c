AuthDigestCred(struct http_auth *ha, Str uname, Str pw, ParsedURL *pu,
	       HRequest *hr, FormList *request)
{
    Str tmp, a1buf, a2buf, rd, s;
    unsigned char md5[MD5_DIGEST_LENGTH + 1];
    Str uri = HTTPrequestURI(pu, hr);
    char nc[] = "00000001";
    FILE *fp;

    Str algorithm = qstr_unquote(get_auth_param(ha->param, "algorithm"));
    Str nonce = qstr_unquote(get_auth_param(ha->param, "nonce"));
    Str cnonce  ;
     
    Str qop = qstr_unquote(get_auth_param(ha->param, "qop"));

    static union {
	int r[4];
	unsigned char s[sizeof(int) * 4];
    } cnonce_seed;
    int qop_i = QOP_NONE;

    cnonce_seed.r[0] = rand();
    cnonce_seed.r[1] = rand();
    cnonce_seed.r[2] = rand();
    MD5(cnonce_seed.s, sizeof(cnonce_seed.s), md5);
    cnonce = digest_hex(md5);
    cnonce_seed.r[3]++;

    if (qop) {
	char *p;
	size_t i;

	p = qop->ptr;
	SKIP_BLANKS(p);

	for (;;) {
	    if ((i = strcspn(p, " \t,")) > 0) {
		if (i == sizeof("auth-int") - sizeof("") && !strncasecmp(p, "auth-int", i)) {
		    if (qop_i < QOP_AUTH_INT)
			qop_i = QOP_AUTH_INT;
		}
		else if (i == sizeof("auth") - sizeof("") && !strncasecmp(p, "auth", i)) {
		    if (qop_i < QOP_AUTH)
			qop_i = QOP_AUTH;
		}
	    }

	    if (p[i]) {
		p += i + 1;
		SKIP_BLANKS(p);
	    }
	    else
		break;
	}
    }

     
    tmp = Strnew_m_charp(uname->ptr, ":",
			 qstr_unquote(get_auth_param(ha->param, "realm"))->ptr,
			 ":", pw->ptr, NULL);
    MD5(tmp->ptr, strlen(tmp->ptr), md5);
    a1buf = digest_hex(md5);

    if (algorithm) {
	if (strcasecmp(algorithm->ptr, "MD5-sess") == 0) {
	     
	    if (nonce == NULL)
		return NULL;
	    tmp = Strnew_m_charp(a1buf->ptr, ":",
				 qstr_unquote(nonce)->ptr,
				 ":", qstr_unquote(cnonce)->ptr, NULL);
	    MD5(tmp->ptr, strlen(tmp->ptr), md5);
	    a1buf = digest_hex(md5);
	}
	else if (strcasecmp(algorithm->ptr, "MD5") == 0)
	     
	    ;
	else
	     
	    return NULL;
    }

     
    tmp = Strnew_m_charp(HTTPrequestMethod(hr)->ptr, ":", uri->ptr, NULL);
    if (qop_i == QOP_AUTH_INT) {
	 
	if (request && request->body) {
	    if (request->method == FORM_METHOD_POST && request->enctype == FORM_ENCTYPE_MULTIPART) {
		fp = fopen(request->body, "r");
		if (fp != NULL) {
		    Str ebody;
		    ebody = Strfgetall(fp);
		    fclose(fp);
		    MD5(ebody->ptr, strlen(ebody->ptr), md5);
		}
		else {
		    MD5("", 0, md5);
		}
	    }
	    else {
		MD5(request->body, request->length, md5);
	    }
	}
	else {
	    MD5("", 0, md5);
	}
	Strcat_char(tmp, ':');
	Strcat(tmp, digest_hex(md5));
    }
    MD5(tmp->ptr, strlen(tmp->ptr), md5);
    a2buf = digest_hex(md5);

    if (qop_i >= QOP_AUTH) {
	 
	if (nonce == NULL)
	    return NULL;
	tmp = Strnew_m_charp(a1buf->ptr, ":", qstr_unquote(nonce)->ptr,
			     ":", nc,
			     ":", qstr_unquote(cnonce)->ptr,
			     ":", qop_i == QOP_AUTH ? "auth" : "auth-int",
			     ":", a2buf->ptr, NULL);
	MD5(tmp->ptr, strlen(tmp->ptr), md5);
	rd = digest_hex(md5);
    }
    else {
	 
	tmp = Strnew_m_charp(a1buf->ptr, ":",
			     qstr_unquote(get_auth_param(ha->param, "nonce"))->
			     ptr, ":", a2buf->ptr, NULL);
	MD5(tmp->ptr, strlen(tmp->ptr), md5);
	rd = digest_hex(md5);
    }

     

    tmp = Strnew_m_charp("Digest username=\"", uname->ptr, "\"", NULL);
    Strcat_m_charp(tmp, ", realm=",
		   get_auth_param(ha->param, "realm")->ptr, NULL);
    Strcat_m_charp(tmp, ", nonce=",
		   get_auth_param(ha->param, "nonce")->ptr, NULL);
    Strcat_m_charp(tmp, ", uri=\"", uri->ptr, "\"", NULL);
    Strcat_m_charp(tmp, ", response=\"", rd->ptr, "\"", NULL);

    if (algorithm)
	Strcat_m_charp(tmp, ", algorithm=",
		       get_auth_param(ha->param, "algorithm")->ptr, NULL);

    if (cnonce)
	Strcat_m_charp(tmp, ", cnonce=\"", cnonce->ptr, "\"", NULL);

    if ((s = get_auth_param(ha->param, "opaque")) != NULL)
	Strcat_m_charp(tmp, ", opaque=", s->ptr, NULL);

    if (qop_i >= QOP_AUTH) {
	Strcat_m_charp(tmp, ", qop=",
		       qop_i == QOP_AUTH ? "auth" : "auth-int",
		       NULL);
	 
	 
	Strcat_m_charp(tmp, ", nc=", nc, NULL);
    }

    return tmp;
}
