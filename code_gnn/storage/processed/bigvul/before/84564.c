panel_set_option(struct parsed_tagarg *arg)
{
    FILE *f = NULL;
    char *p;
    Str s = Strnew(), tmp;

    if (config_file == NULL) {
	disp_message("There's no config file... config not saved", FALSE);
    }
    else {
	f = fopen(config_file, "wt");
	if (f == NULL) {
	    disp_message("Can't write option!", FALSE);
	}
    }
    while (arg) {
	 
	if (arg->value) {
	    p = conv_to_system(arg->value);
	    if (set_param(arg->arg, p)) {
		tmp = Sprintf("%s %s\n", arg->arg, p);
		Strcat(tmp, s);
		s = tmp;
	    }
	}
	arg = arg->next;
    }
    if (f) {
	fputs(s->ptr, f);
	fclose(f);
    }
    sync_with_option();
    backBf();
}