loadSiteconf(void)
{
    char *efname;
    FILE *fp;
    Str line;
    struct siteconf_rec *ent = NULL;

    siteconf_head = NULL;
    if (!siteconf_file)
	return;
    if ((efname = expandPath(siteconf_file)) == NULL)
	return;
    fp = fopen(efname, "r");
    if (fp == NULL)
	return;
    while (line = Strfgets(fp), line->length > 0) {
	char *p, *s;

	Strchop(line);
	p = line->ptr;
	SKIP_BLANKS(p);
	if (*p == '#' || *p == '\0')
	    continue;
	s = getWord(&p);

	 
	if (strcmp(s, "url") == 0) {
	    char *url, *opt;
	    struct siteconf_rec *newent;

	     
	    if (ent) {
		ent->next = siteconf_head;
		siteconf_head = ent;
		ent = NULL;
	    }

	     
	    newent = newSiteconfRec();
	    url = getRegexWord((const char **)&p, &newent->re_url);
	    opt = getWord(&p);
	    SKIP_BLANKS(p);
	    if (!newent->re_url) {
		ParsedURL pu;
		if (!url || !*url)
		    continue;
		parseURL2(url, &pu, NULL);
		newent->url = parsedURL2Str(&pu)->ptr;
	    }
	     
	    if (strcmp(opt, "exact") == 0) {
		newent->url_exact = TRUE;
	    }
	    else if (*opt != 0)
		    continue;
	    if (*p)
		continue;
	    ent = newent;
	    continue;
	}

	 
	if (!ent)
	    continue;

	 
	if (strcmp(s, "substitute_url") == 0) {
	    ent->substitute_url = getQWord(&p);
	    SCONF_SET(ent, SCONF_SUBSTITUTE_URL);
	}
#ifdef USE_M17N
	else if (strcmp(s, "url_charset") == 0) {
	    char *charset = getWord(&p);
	    ent->url_charset = (charset && *charset) ?
		wc_charset_to_ces(charset) : 0;
	    SCONF_SET(ent, SCONF_URL_CHARSET);
	}
#endif  
	else if (strcmp(s, "no_referer_from") == 0) {
	    ent->no_referer_from = str_to_bool(getWord(&p), 0);
	    SCONF_SET(ent, SCONF_NO_REFERER_FROM);
	}
	else if (strcmp(s, "no_referer_to") == 0) {
	    ent->no_referer_to = str_to_bool(getWord(&p), 0);
	    SCONF_SET(ent, SCONF_NO_REFERER_TO);
	}
    }
    if (ent) {
	ent->next = siteconf_head;
	siteconf_head = ent;
	ent = NULL;
    }
    fclose(fp);
}