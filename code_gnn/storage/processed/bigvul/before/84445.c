DEFUN(svBuf, PRINT SAVE_SCREEN, "Save rendered document")
{
    char *qfile = NULL, *file;
    FILE *f;
    int is_pipe;

    CurrentKeyData = NULL;	 
    file = searchKeyData();
    if (file == NULL || *file == '\0') {
	 
	qfile = inputLineHist("Save buffer to: ", NULL, IN_COMMAND, SaveHist);
	if (qfile == NULL || *qfile == '\0') {
	    displayBuffer(Currentbuf, B_NORMAL);
	    return;
	}
    }
    file = conv_to_system(qfile ? qfile : file);
    if (*file == '|') {
	is_pipe = TRUE;
	f = popen(file + 1, "w");
    }
    else {
	if (qfile) {
	    file = unescape_spaces(Strnew_charp(qfile))->ptr;
	    file = conv_to_system(file);
	}
	file = expandPath(file);
	if (checkOverWrite(file) < 0) {
	    displayBuffer(Currentbuf, B_NORMAL);
	    return;
	}
	f = fopen(file, "w");
	is_pipe = FALSE;
    }
    if (f == NULL) {
	 
	char *emsg = Sprintf("Can't open %s", conv_from_system(file))->ptr;
	disp_err_message(emsg, TRUE);
	return;
    }
    saveBuffer(Currentbuf, f, TRUE);
    if (is_pipe)
	pclose(f);
    else
	fclose(f);
    displayBuffer(Currentbuf, B_NORMAL);
}
