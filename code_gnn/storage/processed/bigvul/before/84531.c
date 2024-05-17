query_from_followform(Str *query, FormItemList *fi, int multipart)
{
    FormItemList *f2;
    FILE *body = NULL;

    if (multipart) {
	*query = tmpfname(TMPF_DFL, NULL);
	body = fopen((*query)->ptr, "w");
	if (body == NULL) {
	    return;
	}
	fi->parent->body = (*query)->ptr;
	fi->parent->boundary =
	    Sprintf("------------------------------%d%ld%ld%ld", CurrentPid,
		    fi->parent, fi->parent->body, fi->parent->boundary)->ptr;
    }
    *query = Strnew();
    for (f2 = fi->parent->item; f2; f2 = f2->next) {
	if (f2->name == NULL)
	    continue;
	 
	if (f2->name->length == 0 &&
	    (multipart || f2->type != FORM_INPUT_TEXT))
	    continue;
	switch (f2->type) {
	case FORM_INPUT_RESET:
	     
	    continue;
	case FORM_INPUT_SUBMIT:
	case FORM_INPUT_IMAGE:
	    if (f2 != fi || f2->value == NULL)
		continue;
	    break;
	case FORM_INPUT_RADIO:
	case FORM_INPUT_CHECKBOX:
	    if (!f2->checked)
		continue;
	}
	if (multipart) {
	    if (f2->type == FORM_INPUT_IMAGE) {
		int x = 0, y = 0;
#ifdef USE_IMAGE
		getMapXY(Currentbuf, retrieveCurrentImg(Currentbuf), &x, &y);
#endif
		*query = Strdup(conv_form_encoding(f2->name, fi, Currentbuf));
		Strcat_charp(*query, ".x");
		form_write_data(body, fi->parent->boundary, (*query)->ptr,
				Sprintf("%d", x)->ptr);
		*query = Strdup(conv_form_encoding(f2->name, fi, Currentbuf));
		Strcat_charp(*query, ".y");
		form_write_data(body, fi->parent->boundary, (*query)->ptr,
				Sprintf("%d", y)->ptr);
	    }
	    else if (f2->name && f2->name->length > 0 && f2->value != NULL) {
		 
		*query = conv_form_encoding(f2->value, fi, Currentbuf);
		if (f2->type == FORM_INPUT_FILE)
		    form_write_from_file(body, fi->parent->boundary,
					 conv_form_encoding(f2->name, fi,
							    Currentbuf)->ptr,
					 (*query)->ptr,
					 Str_conv_to_system(f2->value)->ptr);
		else
		    form_write_data(body, fi->parent->boundary,
				    conv_form_encoding(f2->name, fi,
						       Currentbuf)->ptr,
				    (*query)->ptr);
	    }
	}
	else {
	     
	    if (f2->type == FORM_INPUT_IMAGE) {
		int x = 0, y = 0;
#ifdef USE_IMAGE
		getMapXY(Currentbuf, retrieveCurrentImg(Currentbuf), &x, &y);
#endif
		Strcat(*query,
		       Str_form_quote(conv_form_encoding
				      (f2->name, fi, Currentbuf)));
		Strcat(*query, Sprintf(".x=%d&", x));
		Strcat(*query,
		       Str_form_quote(conv_form_encoding
				      (f2->name, fi, Currentbuf)));
		Strcat(*query, Sprintf(".y=%d", y));
	    }
	    else {
		 
		if (f2->name && f2->name->length > 0) {
		    Strcat(*query,
			   Str_form_quote(conv_form_encoding
					  (f2->name, fi, Currentbuf)));
		    Strcat_char(*query, '=');
		}
		if (f2->value != NULL) {
		    if (fi->parent->method == FORM_METHOD_INTERNAL)
			Strcat(*query, Str_form_quote(f2->value));
		    else {
			Strcat(*query,
			       Str_form_quote(conv_form_encoding
					      (f2->value, fi, Currentbuf)));
		    }
		}
	    }
	    if (f2->next)
		Strcat_char(*query, '&');
	}
    }
    if (multipart) {
	fprintf(body, "--%s--\r\n", fi->parent->boundary);
	fclose(body);
    }
    else {
	 
	while (Strlastchar(*query) == '&')
	    Strshrink(*query, 1);
    }
}