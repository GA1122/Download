show_params(FILE * fp)
{
    int i, j, l;
    const char *t = "";
    char *cmt;

#ifdef USE_M17N
#ifdef ENABLE_NLS
    OptionCharset = SystemCharset;	 
#endif
#endif

    fputs("\nconfiguration parameters\n", fp);
    for (j = 0; sections[j].name != NULL; j++) {
#ifdef USE_M17N
	if (!OptionEncode)
	    cmt =
		wc_conv(_(sections[j].name), OptionCharset,
			InnerCharset)->ptr;
	else
#endif
	    cmt = sections[j].name;
	fprintf(fp, "  section[%d]: %s\n", j, conv_to_system(cmt));
	i = 0;
	while (sections[j].params[i].name) {
	    switch (sections[j].params[i].type) {
	    case P_INT:
	    case P_SHORT:
	    case P_CHARINT:
	    case P_NZINT:
		t = (sections[j].params[i].inputtype ==
		     PI_ONOFF) ? "bool" : "number";
		break;
	    case P_CHAR:
		t = "char";
		break;
	    case P_STRING:
		t = "string";
		break;
#if defined(USE_SSL) && defined(USE_SSL_VERIFY)
	    case P_SSLPATH:
		t = "path";
		break;
#endif
#ifdef USE_COLOR
	    case P_COLOR:
		t = "color";
		break;
#endif
#ifdef USE_M17N
	    case P_CODE:
		t = "charset";
		break;
#endif
	    case P_PIXELS:
		t = "number";
		break;
	    case P_SCALE:
		t = "percent";
		break;
	    }
#ifdef USE_M17N
	    if (!OptionEncode)
		cmt = wc_conv(_(sections[j].params[i].comment),
			      OptionCharset, InnerCharset)->ptr;
	    else
#endif
		cmt = sections[j].params[i].comment;
	    l = 30 - (strlen(sections[j].params[i].name) + strlen(t));
	    if (l < 0)
		l = 1;
	    fprintf(fp, "    -o %s=<%s>%*s%s\n",
		    sections[j].params[i].name, t, l, " ",
		    conv_to_system(cmt));
	    i++;
	}
    }
}