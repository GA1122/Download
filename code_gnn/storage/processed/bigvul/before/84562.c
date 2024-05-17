load_option_panel(void)
{
    Str src;
    struct param_ptr *p;
    struct sel_c *s;
#ifdef USE_M17N
    wc_ces_list *c;
#endif
    int x, i;
    Str tmp;
    Buffer *buf;

    if (optionpanel_str == NULL)
	optionpanel_str = Sprintf(optionpanel_src1, w3m_version,
			      html_quote(localCookie()->ptr), _(CMT_HELPER));
#ifdef USE_M17N
#ifdef ENABLE_NLS
    OptionCharset = SystemCharset;	 
#endif
    if (!OptionEncode) {
	optionpanel_str =
	    wc_Str_conv(optionpanel_str, OptionCharset, InnerCharset);
	for (i = 0; sections[i].name != NULL; i++) {
	    sections[i].name =
		wc_conv(_(sections[i].name), OptionCharset,
			InnerCharset)->ptr;
	    for (p = sections[i].params; p->name; p++) {
		p->comment =
		    wc_conv(_(p->comment), OptionCharset,
			    InnerCharset)->ptr;
		if (p->inputtype == PI_SEL_C
#ifdef USE_COLOR
			&& p->select != colorstr
#endif
			) {
		    for (s = (struct sel_c *)p->select; s->text != NULL; s++) {
			s->text =
			    wc_conv(_(s->text), OptionCharset,
				    InnerCharset)->ptr;
		    }
		}
	    }
	}
#ifdef USE_COLOR
	for (s = colorstr; s->text; s++)
	    s->text = wc_conv(_(s->text), OptionCharset,
			      InnerCharset)->ptr;
#endif
	OptionEncode = TRUE;
    }
#endif
    src = Strdup(optionpanel_str);

    Strcat_charp(src, "<table><tr><td>");
    for (i = 0; sections[i].name != NULL; i++) {
	Strcat_m_charp(src, "<h1>", sections[i].name, "</h1>", NULL);
	p = sections[i].params;
	Strcat_charp(src, "<table width=100% cellpadding=0>");
	while (p->name) {
	    Strcat_m_charp(src, "<tr><td>", p->comment, NULL);
	    Strcat(src, Sprintf("</td><td width=%d>",
				(int)(28 * pixel_per_char)));
	    switch (p->inputtype) {
	    case PI_TEXT:
		Strcat_m_charp(src, "<input type=text name=",
			       p->name,
			       " value=\"",
			       html_quote(to_str(p)->ptr), "\">", NULL);
		break;
	    case PI_ONOFF:
		x = atoi(to_str(p)->ptr);
		Strcat_m_charp(src, "<input type=radio name=",
			       p->name,
			       " value=1",
			       (x ? " checked" : ""),
			       ">YES&nbsp;&nbsp;<input type=radio name=",
			       p->name,
			       " value=0", (x ? "" : " checked"), ">NO", NULL);
		break;
	    case PI_SEL_C:
		tmp = to_str(p);
		Strcat_m_charp(src, "<select name=", p->name, ">", NULL);
		for (s = (struct sel_c *)p->select; s->text != NULL; s++) {
		    Strcat_charp(src, "<option value=");
		    Strcat(src, Sprintf("%s\n", s->cvalue));
		    if ((p->type != P_CHAR && s->value == atoi(tmp->ptr)) ||
			(p->type == P_CHAR && (char)s->value == *(tmp->ptr)))
			Strcat_charp(src, " selected");
		    Strcat_char(src, '>');
		    Strcat_charp(src, s->text);
		}
		Strcat_charp(src, "</select>");
		break;
#ifdef USE_M17N
	    case PI_CODE:
		tmp = to_str(p);
		Strcat_m_charp(src, "<select name=", p->name, ">", NULL);
		for (c = *(wc_ces_list **) p->select; c->desc != NULL; c++) {
		    Strcat_charp(src, "<option value=");
		    Strcat(src, Sprintf("%s\n", c->name));
		    if (c->id == atoi(tmp->ptr))
			Strcat_charp(src, " selected");
		    Strcat_char(src, '>');
		    Strcat_charp(src, c->desc);
		}
		Strcat_charp(src, "</select>");
		break;
#endif
	    }
	    Strcat_charp(src, "</td></tr>\n");
	    p++;
	}
	Strcat_charp(src,
		     "<tr><td></td><td><p><input type=submit value=\"OK\"></td></tr>");
	Strcat_charp(src, "</table><hr width=50%>");
    }
    Strcat_charp(src, "</table></form></body></html>");
    buf = loadHTMLString(src);
#ifdef USE_M17N
    if (buf)
	buf->document_charset = OptionCharset;
#endif
    return buf;
}