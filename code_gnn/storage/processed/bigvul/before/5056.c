AddDef(String *buff, char *title, char *value)
{
#ifdef PATHETICCPP
    if (need_real_defines) {
	addstring(buff, "\n#define ");
	addstring(buff, title);
	if (value && (value[0] != '\0')) {
	    addstring(buff, " ");
	    addstring(buff, value);
	}
	return;
    }
#endif
    if (buff->used) {
	if (oper == OPSYMBOLS)
	    addstring(buff, "\n-D");
	else
	    addstring(buff, " -D");
    } else
 	    addstring(buff, " -D");
     } else
