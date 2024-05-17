sync_with_option(void)
{
    if (PagerMax < LINES)
	PagerMax = LINES;
    WrapSearch = WrapDefault;
    parse_proxy();
#ifdef USE_COOKIE
    parse_cookie();
#endif
    initMailcap();
    initMimeTypes();
#ifdef USE_EXTERNAL_URI_LOADER
    initURIMethods();
#endif
#ifdef USE_MIGEMO
    init_migemo();
#endif
#ifdef USE_IMAGE
    if (fmInitialized && displayImage)
	initImage();
#else
    displayImage = FALSE;	 
#endif
    loadPasswd();
    loadPreForm();
    loadSiteconf();

    if (AcceptLang == NULL || *AcceptLang == '\0') {
	 
	AcceptLang = _("en;q=1.0");
    }
    if (AcceptEncoding == NULL || *AcceptEncoding == '\0')
	AcceptEncoding = acceptableEncoding();
    if (AcceptMedia == NULL || *AcceptMedia == '\0')
	AcceptMedia = acceptableMimeTypes();
#ifdef USE_UNICODE
    update_utf8_symbol();
#endif
    if (fmInitialized) {
	initKeymap(FALSE);
#ifdef USE_MOUSE
	initMouseAction();
#endif				 
#ifdef USE_MENU
	initMenu();
#endif				 
    }
}
