_quitfm(int confirm)
{
    char *ans = "y";

    if (checkDownloadList())
	 
	ans = inputChar("Download process retains. "
			"Do you want to exit w3m? (y/n)");
    else if (confirm)
	 
	ans = inputChar("Do you want to exit w3m? (y/n)");
    if (!(ans && TOLOWER(*ans) == 'y')) {
	displayBuffer(Currentbuf, B_NORMAL);
	return;
    }

    term_title("");		 
#ifdef USE_IMAGE
    if (activeImage)
	termImage();
#endif
    fmTerm();
#ifdef USE_COOKIE
    save_cookies();
#endif				 
#ifdef USE_HISTORY
    if (UseHistory && SaveURLHist)
	saveHistory(URLHist, URLHistSize);
#endif				 
    w3m_exit(0);
}
