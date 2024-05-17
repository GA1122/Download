invoke_browser(char *url)
{
    Str cmd;
    char *browser = NULL;
    int bg = 0, len;

    CurrentKeyData = NULL;	 
    browser = searchKeyData();
    if (browser == NULL || *browser == '\0') {
	switch (prec_num) {
	case 0:
	case 1:
	    browser = ExtBrowser;
	    break;
	case 2:
	    browser = ExtBrowser2;
	    break;
	case 3:
	    browser = ExtBrowser3;
	    break;
	case 4:
	    browser = ExtBrowser4;
	    break;
	case 5:
	    browser = ExtBrowser5;
	    break;
	case 6:
	    browser = ExtBrowser6;
	    break;
	case 7:
	    browser = ExtBrowser7;
	    break;
	case 8:
	    browser = ExtBrowser8;
	    break;
	case 9:
	    browser = ExtBrowser9;
	    break;
	}
	if (browser == NULL || *browser == '\0') {
	    browser = inputStr("Browse command: ", NULL);
	    if (browser != NULL)
		browser = conv_to_system(browser);
	}
    }
    else {
	browser = conv_to_system(browser);
    }
    if (browser == NULL || *browser == '\0') {
	displayBuffer(Currentbuf, B_NORMAL);
	return;
    }

    if ((len = strlen(browser)) >= 2 && browser[len - 1] == '&' &&
	browser[len - 2] != '\\') {
	browser = allocStr(browser, len - 2);
	bg = 1;
    }
    cmd = myExtCommand(browser, shell_quote(url), FALSE);
    Strremovetrailingspaces(cmd);
    fmTerm();
    mySystem(cmd->ptr, bg);
    fmInit();
    displayBuffer(Currentbuf, B_FORCE_REDRAW);
}