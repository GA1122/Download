DEFUN(susp, INTERRUPT SUSPEND, "Suspend w3m to background")
{
#ifndef SIGSTOP
    char *shell;
#endif				 
    move(LASTLINE, 0);
    clrtoeolx();
    refresh();
    fmTerm();
#ifndef SIGSTOP
    shell = getenv("SHELL");
    if (shell == NULL)
	shell = "/bin/sh";
    system(shell);
#else				 
#ifdef SIGTSTP
    signal(SIGTSTP, SIG_DFL);   
     
    kill(0, SIGTSTP);   
#else
    kill((pid_t) 0, SIGSTOP);
#endif
#endif				 
    fmInit();
    displayBuffer(Currentbuf, B_FORCE_REDRAW);
}
