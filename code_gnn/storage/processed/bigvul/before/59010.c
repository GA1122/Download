static int HTML_start_element(HTStructured * me, int element_number,
			      const BOOL *present,
			      STRING2PTR value,
			      int tag_charset,
			      char **include)
{
    char *alt_string = NULL;
    char *id_string = NULL;
    char *newtitle = NULL;
    char **pdoctitle = NULL;
    char *href = NULL;
    char *map_href = NULL;
    char *title = NULL;
    char *I_value = NULL;
    char *I_name = NULL;
    char *temp = NULL;
    const char *Base = NULL;
    int dest_char_set = -1;
    HTParentAnchor *dest = NULL;	 
    BOOL dest_ismap = FALSE;	 
    HTChildAnchor *ID_A = NULL;	 
    int url_type = 0, i = 0;
    char *cp = NULL;
    HTMLElement ElementNumber = (HTMLElement) element_number;
    BOOL intern_flag = FALSE;
    short stbl_align = HT_ALIGN_NONE;
    int status = HT_OK;

#ifdef USE_COLOR_STYLE
    char *class_name;
    int class_used = 0;
#endif

#ifdef USE_PRETTYSRC
    if (psrc_view && !sgml_in_psrc_was_initialized) {
	if (!psrc_nested_call) {
	    HTTag *tag = &HTML_dtd.tags[element_number];
	    char buf[200];
	    const char *p;

	    if (psrc_first_tag) {
		psrc_first_tag = FALSE;
		 
		psrc_nested_call = TRUE;
		HTML_start_element(me, HTML_BODY, NULL, NULL, tag_charset, NULL);
		HTML_start_element(me, HTML_PRE, NULL, NULL, tag_charset, NULL);
		PSRCSTART(entire);
		psrc_nested_call = FALSE;
	    }

	    psrc_nested_call = TRUE;
	     
	    PSRCSTART(abracket);
	    PUTC('<');
	    PSRCSTOP(abracket);
	    PSRCSTART(tag);
	    if (tagname_transform != 0)
		PUTS(tag->name);
	    else {
		LYStrNCpy(buf, tag->name, sizeof(buf) - 1);
		LYLowerCase(buf);
		PUTS(buf);
	    }
	    if (present) {
		for (i = 0; i < tag->number_of_attributes; i++)
		    if (present[i]) {
			PUTC(' ');
			PSRCSTART(attrib);
			if (attrname_transform != 0)
			    PUTS(tag->attributes[i].name);
			else {
			    LYStrNCpy(buf,
				      tag->attributes[i].name,
				      sizeof(buf) - 1);
			    LYLowerCase(buf);
			    PUTS(buf);
			}
			if (value[i]) {
			    char q = '"';

			     
			    char kind = (char) (!StrChr(value[i], '"') ?
						0 :
						!StrChr(value[i], '\'') ?
						q = '\'', 1 :
						2);

			    PUTC('=');
			    PSRCSTOP(attrib);
			    PSRCSTART(attrval);
			    PUTC(q);
			     
			    if (tag->attributes[i].type == HTMLA_ANAME) {
				HTStartAnchor(me, value[i], NULL);
				HTML_end_element(me, HTML_A, NULL);
			    } else if (tag->attributes[i].type == HTMLA_HREF) {
				PSRCSTART(href);
				HTStartAnchor(me, NULL, value[i]);
			    }
			    if (kind != 2)
				PUTS(value[i]);
			    else
				for (p = value[i]; *p; p++)
				    if (*p != '"')
					PUTC(*p);
				    else
					PUTS("&#34;");
			     
			    if (tag->attributes[i].type == HTMLA_HREF) {
				HTML_end_element(me, HTML_A, NULL);
				PSRCSTOP(href);
			    }
			    PUTC(q);
			    PSRCSTOP(attrval);
			}	 
		    }		 
	    }			 
	    PSRCSTOP(tag);
	    PSRCSTART(abracket);
	    PUTC('>');
	    PSRCSTOP(abracket);
	    psrc_nested_call = FALSE;
	    return HT_OK;
	}			 
	 
    }
#endif  

    if (LYMapsOnly) {
	if (!(ElementNumber == HTML_MAP || ElementNumber == HTML_AREA ||
	      ElementNumber == HTML_BASE || ElementNumber == HTML_OBJECT ||
	      ElementNumber == HTML_A)) {
	    return HT_OK;
	}
    } else if (!me->text) {
	UPDATE_STYLE;
    } {
	 
	int j = ((tag_charset < 0) ? me->UCLYhndl : tag_charset);

	if ((me->tag_charset != j) || (j < 0   )) {
	    CTRACE((tfp, "me->tag_charset: %d -> %d", me->tag_charset, j));
	    CTRACE((tfp, " (me->UCLYhndl: %d, tag_charset: %d)\n",
		    me->UCLYhndl, tag_charset));
	    me->tag_charset = j;
	}
    }

 
#if defined(USE_COLOR_STYLE)

    addClassName(";",
		 HTML_dtd.tags[element_number].name,
		 (size_t) HTML_dtd.tags[element_number].name_len);

    class_name = (force_classname ? forced_classname : class_string);
    force_classname = FALSE;

    if (force_current_tag_style == FALSE) {
	current_tag_style = (class_name[0]
			     ? -1
			     : cached_tag_styles[element_number]);
    } else {
	force_current_tag_style = FALSE;
    }

    CTRACE2(TRACE_STYLE, (tfp, "CSS.elt:<%s>\n", HTML_dtd.tags[element_number].name));

    if (current_tag_style == -1) {	 
	hcode = hash_code_lowercase_on_fly(HTML_dtd.tags[element_number].name);
	if (class_name[0]) {
	    int ohcode = hcode;

	    hcode = hash_code_aggregate_char('.', hcode);
	    hcode = hash_code_aggregate_lower_str(class_name, hcode);
	    if (!hashStyles[hcode].name) {	 
		hcode = ohcode;
		CTRACE2(TRACE_STYLE,
			(tfp,
			 "STYLE.start_element: <%s> (class <%s> not configured), hcode=%d.\n",
			 HTML_dtd.tags[element_number].name, class_name, hcode));
	    } else {
		addClassName(".", class_name, strlen(class_name));

		CTRACE2(TRACE_STYLE,
			(tfp, "STYLE.start_element: <%s>.<%s>, hcode=%d.\n",
			 HTML_dtd.tags[element_number].name, class_name, hcode));
		class_used = 1;
	    }
	}

	class_string[0] = '\0';

    } else {			 
	if (class_name[0]) {
	    addClassName(".", class_name, strlen(class_name));
	    class_string[0] = '\0';
	}
	hcode = current_tag_style;
	CTRACE2(TRACE_STYLE,
		(tfp, "STYLE.start_element: <%s>, hcode=%d.\n",
		 HTML_dtd.tags[element_number].name, hcode));
	current_tag_style = -1;
    }

#if !OMIT_SCN_KEEPING		 
    if (!class_used && ElementNumber == HTML_INPUT) {	 
	const char *type = "";
	int ohcode = hcode;

	if (present && present[HTML_INPUT_TYPE] && value[HTML_INPUT_TYPE])
	    type = value[HTML_INPUT_TYPE];

	hcode = hash_code_aggregate_lower_str(".type.", hcode);
	hcode = hash_code_aggregate_lower_str(type, hcode);
	if (!hashStyles[hcode].name) {	 
	    hcode = ohcode;
	    CTRACE2(TRACE_STYLE,
		    (tfp, "STYLE.start_element: type <%s> not configured.\n",
		     type));
	} else {
	    addClassName(".type.", type, strlen(type));

	    CTRACE2(TRACE_STYLE,
		    (tfp, "STYLE.start_element: <%s>.type.<%s>, hcode=%d.\n",
		     HTML_dtd.tags[element_number].name, type, hcode));
	}
    }
#endif  

    HText_characterStyle(me->text, hcode, STACK_ON);
#endif  

     
    switch (ElementNumber) {

    case HTML_HTML:
	break;

    case HTML_HEAD:
	break;

    case HTML_BASE:
	if (present && present[HTML_BASE_HREF] && !local_host_only &&
	    non_empty(value[HTML_BASE_HREF])) {
	    char *base = NULL;
	    const char *related = NULL;

	    StrAllocCopy(base, value[HTML_BASE_HREF]);
	    CTRACE((tfp, "*HTML_BASE: initial href=`%s'\n", NonNull(base)));

	    if (!(url_type = LYLegitimizeHREF(me, &base, TRUE, TRUE))) {
		CTRACE((tfp, "HTML: BASE '%s' is not an absolute URL.\n",
			NonNull(base)));
		if (me->inBadBASE == FALSE)
		    HTAlert(BASE_NOT_ABSOLUTE);
		me->inBadBASE = TRUE;
	    }

	    if (url_type == LYNXIMGMAP_URL_TYPE) {
		 
		temp = base;
		base = HTParse(base + 11, "", PARSE_ALL_WITHOUT_ANCHOR);
		FREE(temp);
	    }

	     
	    related = me->node_anchor->address;

	     
	    temp = HTParse(base, related, PARSE_ACCESS + PARSE_PUNCTUATION);
	    StrAllocCopy(me->base_href, temp);
	    FREE(temp);

	     
	    temp = HTParse(base, "", PARSE_HOST + PARSE_PUNCTUATION);
	    if (!StrNCmp(temp, "//", 2)) {
		StrAllocCat(me->base_href, temp);
		if (!strcmp(me->base_href, "file://")) {
		    StrAllocCat(me->base_href, "localhost");
		}
	    } else {
		if (isFILE_URL(me->base_href)) {
		    StrAllocCat(me->base_href, "//localhost");
		} else if (strcmp(me->base_href, STR_NEWS_URL)) {
		    FREE(temp);
		    StrAllocCat(me->base_href, (temp = HTParse(related, "",
							       PARSE_HOST + PARSE_PUNCTUATION)));
		}
	    }
	    FREE(temp);

	     
	    temp = HTParse(base, "", PARSE_PATH + PARSE_PUNCTUATION);
	    if (*temp != '\0') {
		char *p = StrChr(temp, '?');

		if (p)
		    *p = '\0';
		p = strrchr(temp, '/');
		if (p)
		    *(p + 1) = '\0';	 

		StrAllocCat(me->base_href, temp);
	    } else if (!strcmp(me->base_href, STR_NEWS_URL)) {
		StrAllocCat(me->base_href, "*");
	    } else if (isNEWS_URL(me->base_href) ||
		       isNNTP_URL(me->base_href) ||
		       isSNEWS_URL(me->base_href)) {
		StrAllocCat(me->base_href, "/*");
	    } else {
		StrAllocCat(me->base_href, "/");
	    }
	    FREE(temp);
	    FREE(base);

	    me->inBASE = TRUE;
	    me->node_anchor->inBASE = TRUE;
	    StrAllocCopy(me->node_anchor->content_base, me->base_href);
	     

	    CTRACE((tfp, "*HTML_BASE: final href=`%s'\n", me->base_href));
	}
	break;

    case HTML_META:
	if (present)
	    LYHandleMETA(me, present, value, include);
	break;

    case HTML_TITLE:
	HTChunkClear(&me->title);
	break;

    case HTML_LINK:
	intern_flag = FALSE;
	if (present && present[HTML_LINK_HREF]) {
	    CHECK_FOR_INTERN(intern_flag, value[HTML_LINK_HREF]);
	     
	    if (isEmpty(value[HTML_LINK_HREF])) {
		Base = (me->inBASE)
		    ? me->base_href
		    : me->node_anchor->address;
		StrAllocCopy(href, Base);
	    } else {
		StrAllocCopy(href, value[HTML_LINK_HREF]);
		(void) LYLegitimizeHREF(me, &href, TRUE, TRUE);

		Base = (me->inBASE && *href != '\0' && *href != '#')
		    ? me->base_href
		    : me->node_anchor->address;
		HTParseALL(&href, Base);
	    }

	     
	    if (present &&
		((present[HTML_LINK_REV] &&
		  value[HTML_LINK_REV] &&
		  (!strcasecomp("made", value[HTML_LINK_REV]) ||
		   !strcasecomp("owner", value[HTML_LINK_REV]))) ||
		 (present[HTML_LINK_REL] &&
		  value[HTML_LINK_REL] &&
		  (!strcasecomp("author", value[HTML_LINK_REL]))))) {
		 
		HTAnchor_setOwner(me->node_anchor, href);
		CTRACE((tfp, "HTML: DOC OWNER '%s' found\n", href));
		FREE(href);

		 
		if (present && present[HTML_LINK_TITLE] &&
		    value[HTML_LINK_TITLE] &&
		    *value[HTML_LINK_TITLE] != '\0') {
		    StrAllocCopy(title, value[HTML_LINK_TITLE]);
		    TRANSLATE_AND_UNESCAPE_ENTITIES(&title, TRUE, FALSE);
		    LYTrimHead(title);
		    LYTrimTail(title);
		    if (*title != '\0')
			HTAnchor_setRevTitle(me->node_anchor, title);
		    FREE(title);
		}
		break;
	    }

	     

	    if (present &&
		present[HTML_LINK_REL] && value[HTML_LINK_REL]) {
		 
#ifndef USE_COLOR_STYLE
		if (!strcasecomp(value[HTML_LINK_REL], "StyleSheet") ||
		    !strcasecomp(value[HTML_LINK_REL], "Style")) {
		    CTRACE2(TRACE_STYLE,
			    (tfp, "HTML: StyleSheet link found.\n"));
		    CTRACE2(TRACE_STYLE,
			    (tfp, "        StyleSheets not yet implemented.\n"));
		    FREE(href);
		    break;
		}
#endif  

		 
		if (!strcasecomp(value[HTML_LINK_REL], "Home") ||
		    !strcasecomp(value[HTML_LINK_REL], "ToC") ||
		    !strcasecomp(value[HTML_LINK_REL], "Contents") ||
		    !strcasecomp(value[HTML_LINK_REL], "Index") ||
		    !strcasecomp(value[HTML_LINK_REL], "Glossary") ||
		    !strcasecomp(value[HTML_LINK_REL], "Copyright") ||
		    !strcasecomp(value[HTML_LINK_REL], "Help") ||
		    !strcasecomp(value[HTML_LINK_REL], "Search") ||
		    !strcasecomp(value[HTML_LINK_REL], "Bookmark") ||
		    !strcasecomp(value[HTML_LINK_REL], "Banner") ||
		    !strcasecomp(value[HTML_LINK_REL], "Top") ||
		    !strcasecomp(value[HTML_LINK_REL], "Origin") ||
		    !strcasecomp(value[HTML_LINK_REL], "Navigator") ||
		    !strcasecomp(value[HTML_LINK_REL], "Disclaimer") ||
		    !strcasecomp(value[HTML_LINK_REL], "Author") ||
		    !strcasecomp(value[HTML_LINK_REL], "Editor") ||
		    !strcasecomp(value[HTML_LINK_REL], "Publisher") ||
		    !strcasecomp(value[HTML_LINK_REL], "Trademark") ||
		    !strcasecomp(value[HTML_LINK_REL], "Hotlist") ||
		    !strcasecomp(value[HTML_LINK_REL], "Begin") ||
		    !strcasecomp(value[HTML_LINK_REL], "First") ||
		    !strcasecomp(value[HTML_LINK_REL], "End") ||
		    !strcasecomp(value[HTML_LINK_REL], "Last") ||
		    !strcasecomp(value[HTML_LINK_REL], "Documentation") ||
		    !strcasecomp(value[HTML_LINK_REL], "Biblioentry") ||
		    !strcasecomp(value[HTML_LINK_REL], "Bibliography") ||
		    !strcasecomp(value[HTML_LINK_REL], "Start") ||
		    !strcasecomp(value[HTML_LINK_REL], "Appendix")) {
		    StrAllocCopy(title, value[HTML_LINK_REL]);
		    pdoctitle = &title;		 
		} else if (!strcasecomp(value[HTML_LINK_REL], "Up") ||
			   !strcasecomp(value[HTML_LINK_REL], "Next") ||
			   !strcasecomp(value[HTML_LINK_REL], "Previous") ||
			   !strcasecomp(value[HTML_LINK_REL], "Prev") ||
			   !strcasecomp(value[HTML_LINK_REL], "Child") ||
			   !strcasecomp(value[HTML_LINK_REL], "Sibling") ||
			   !strcasecomp(value[HTML_LINK_REL], "Parent") ||
			   !strcasecomp(value[HTML_LINK_REL], "Meta") ||
			   !strcasecomp(value[HTML_LINK_REL], "URC") ||
			   !strcasecomp(value[HTML_LINK_REL], "Pointer") ||
			   !strcasecomp(value[HTML_LINK_REL], "Translation") ||
			   !strcasecomp(value[HTML_LINK_REL], "Definition") ||
			   !strcasecomp(value[HTML_LINK_REL], "Alternate") ||
			   !strcasecomp(value[HTML_LINK_REL], "Section") ||
			   !strcasecomp(value[HTML_LINK_REL], "Subsection") ||
			   !strcasecomp(value[HTML_LINK_REL], "Chapter")) {
		    StrAllocCopy(title, value[HTML_LINK_REL]);
		     
		    if (!intern_flag) {
			StrAllocCopy(temp, "RelTitle: ");
			StrAllocCat(temp, value[HTML_LINK_REL]);
		    }
#ifndef DISABLE_BIBP
		} else if (!strcasecomp(value[HTML_LINK_REL], "citehost")) {
		     
		    HTAnchor_setCitehost(me->node_anchor, href);
		    CTRACE((tfp, "HTML: citehost '%s' found\n", href));
		    FREE(href);
		    break;
#endif
		} else {
		    CTRACE((tfp, "HTML: LINK with REL=\"%s\" ignored.\n",
			    value[HTML_LINK_REL]));
		    FREE(href);
		    break;
		}
	    }
	} else if (present &&
		   present[HTML_LINK_REL] && value[HTML_LINK_REL]) {
	     
	    if (!strcasecomp(value[HTML_LINK_REL], "Home")) {
		StrAllocCopy(href, LynxHome);
	    } else if (!strcasecomp(value[HTML_LINK_REL], "Help")) {
		StrAllocCopy(href, helpfile);
	    } else if (!strcasecomp(value[HTML_LINK_REL], "Index")) {
		StrAllocCopy(href, indexfile);
	    } else {
		CTRACE((tfp,
			"HTML: LINK with REL=\"%s\" and no HREF ignored.\n",
			value[HTML_LINK_REL]));
		break;
	    }
	    StrAllocCopy(title, value[HTML_LINK_REL]);
	    pdoctitle = &title;
	}
	if (href) {
	     
	    if (present && present[HTML_LINK_TITLE] &&
		non_empty(value[HTML_LINK_TITLE])) {
		StrAllocCopy(title, value[HTML_LINK_TITLE]);
		TRANSLATE_AND_UNESCAPE_ENTITIES(&title, TRUE, FALSE);
		LYTrimHead(title);
		LYTrimTail(title);
		pdoctitle = &title;
		FREE(temp);	 
	    }
	    if (isEmpty(title)) {
		FREE(href);
		FREE(title);
		break;
	    }

	    if (me->inA) {
		 
		SET_SKIP_STACK(HTML_A);
		HTML_end_element(me, HTML_A, include);
	    }

	     
	    me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
						     NULL,	 
						     href,	 
						     (temp
						      ? (HTLinkType *)
						      HTAtom_for(temp)
						      : INTERN_LT));	 
	    FREE(temp);
	    if ((dest = HTAnchor_parent(HTAnchor_followLink(me->CurrentA)
		 )) != NULL) {
		if (pdoctitle && !HTAnchor_title(dest))
		    HTAnchor_setTitle(dest, *pdoctitle);

		 
		if (dest == me->node_anchor)
		    dest = NULL;
		if (present[HTML_LINK_CHARSET] &&
		    non_empty(value[HTML_LINK_CHARSET])) {
		    dest_char_set = UCGetLYhndl_byMIME(value[HTML_LINK_CHARSET]);
		    if (dest_char_set < 0)
			dest_char_set = UCLYhndl_for_unrec;
		}
		if (dest && dest_char_set >= 0)
		    HTAnchor_setUCInfoStage(dest, dest_char_set,
					    UCT_STAGE_PARSER,
					    UCT_SETBY_LINK);
	    }
	    UPDATE_STYLE;
	    if (!HText_hasToolbar(me->text) &&
		(ID_A = HTAnchor_findChildAndLink(me->node_anchor,	 
						  LYToolbarName,	 
						  NULL,		 
						  (HTLinkType *) 0))) {		 
		HText_appendCharacter(me->text, '#');
		HText_setLastChar(me->text, ' ');	 
		HText_beginAnchor(me->text, me->inUnderline, ID_A);
		HText_endAnchor(me->text, 0);
		HText_setToolbar(me->text);
	    } else {
		 
		HTML_put_character(me, ' ');
	    }
	    HText_beginAnchor(me->text, me->inUnderline, me->CurrentA);
	    if (me->inBoldH == FALSE)
		HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
#ifdef USE_COLOR_STYLE
	    if (present && present[HTML_LINK_CLASS] &&
		non_empty(value[HTML_LINK_CLASS])) {
		char *tmp = 0;

		HTSprintf0(&tmp, "link.%s.%s", value[HTML_LINK_CLASS], title);
		CTRACE2(TRACE_STYLE,
			(tfp, "STYLE.link: using style <%s>\n", tmp));

		HText_characterStyle(me->text, hash_code(tmp), STACK_ON);
		HTML_put_string(me, title);
		HTML_put_string(me, " (");
		HTML_put_string(me, value[HTML_LINK_CLASS]);
		HTML_put_string(me, ")");
		HText_characterStyle(me->text, hash_code(tmp), STACK_OFF);
		FREE(tmp);
	    } else
#endif
		HTML_put_string(me, title);
	    if (me->inBoldH == FALSE)
		HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
	    HText_endAnchor(me->text, 0);
	}
	FREE(href);
	FREE(title);
	break;

    case HTML_ISINDEX:
	if (((present)) &&
	    ((present[HTML_ISINDEX_HREF] && value[HTML_ISINDEX_HREF]) ||
	     (present[HTML_ISINDEX_ACTION] && value[HTML_ISINDEX_ACTION]))) {
	     
	    if (present[HTML_ISINDEX_HREF] && value[HTML_ISINDEX_HREF])
		StrAllocCopy(href, value[HTML_ISINDEX_HREF]);
	    else
		StrAllocCopy(href, value[HTML_ISINDEX_ACTION]);
	    LYLegitimizeHREF(me, &href, TRUE, TRUE);

	    Base = (me->inBASE && *href != '\0' && *href != '#')
		? me->base_href
		: me->node_anchor->address;
	    HTParseALL(&href, Base);
	    HTAnchor_setIndex(me->node_anchor, href);
	    FREE(href);

	} else {
	    Base = (me->inBASE) ?
		me->base_href : me->node_anchor->address;
	    HTAnchor_setIndex(me->node_anchor, Base);
	}
	 
	if (present &&
	    present[HTML_ISINDEX_PROMPT] &&
	    non_empty(value[HTML_ISINDEX_PROMPT])) {
	    StrAllocCopy(temp, value[HTML_ISINDEX_PROMPT]);
	    TRANSLATE_AND_UNESCAPE_ENTITIES(&temp, TRUE, FALSE);
	    LYTrimHead(temp);
	    LYTrimTail(temp);
	    if (*temp != '\0') {
		StrAllocCat(temp, " ");
		HTAnchor_setPrompt(me->node_anchor, temp);
	    } else {
		HTAnchor_setPrompt(me->node_anchor, ENTER_DATABASE_QUERY);
	    }
	    FREE(temp);
	} else {
	    HTAnchor_setPrompt(me->node_anchor, ENTER_DATABASE_QUERY);
	}
	break;

    case HTML_NEXTID:
	break;

    case HTML_STYLE:
	 
	HTChunkClear(&me->style_block);
	break;

    case HTML_SCRIPT:
	 
	HTChunkClear(&me->script);
	break;

    case HTML_BODY:
	CHECK_ID(HTML_BODY_ID);
	if (HText_hasToolbar(me->text))
	    HText_appendParagraph(me->text);
	break;

    case HTML_SECTION:
    case HTML_ARTICLE:
    case HTML_MAIN:
    case HTML_ASIDE:
    case HTML_HEADER:
    case HTML_FOOTER:
    case HTML_NAV:
	CHECK_ID(HTML_GEN5_ID);
	if (HText_hasToolbar(me->text))
	    HText_appendParagraph(me->text);
	break;

    case HTML_FIGURE:
	CHECK_ID(HTML_GEN5_ID);
	break;

    case HTML_FRAMESET:
	break;

    case HTML_FRAME:
	if (present && present[HTML_FRAME_NAME] &&
	    non_empty(value[HTML_FRAME_NAME])) {
	    StrAllocCopy(id_string, value[HTML_FRAME_NAME]);
	    TRANSLATE_AND_UNESCAPE_ENTITIES(&id_string, TRUE, FALSE);
	    LYTrimHead(id_string);
	    LYTrimTail(id_string);
	}
	if (present && present[HTML_FRAME_SRC] &&
	    non_empty(value[HTML_FRAME_SRC])) {
	    StrAllocCopy(href, value[HTML_FRAME_SRC]);
	    LYLegitimizeHREF(me, &href, TRUE, TRUE);

	    if (me->inA) {
		SET_SKIP_STACK(HTML_A);
		HTML_end_element(me, HTML_A, include);
	    }
	    me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
						     NULL,	 
						     href,	 
						     (HTLinkType *) 0);		 
	    CAN_JUSTIFY_PUSH(FALSE);
	    LYEnsureSingleSpace(me);
	    if (me->inUnderline == FALSE)
		HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	    HTML_put_string(me, "FRAME:");
	    if (me->inUnderline == FALSE)
		HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	    HTML_put_character(me, ' ');

	    me->in_word = NO;
	    CHECK_ID(HTML_FRAME_ID);
	    HText_beginAnchor(me->text, me->inUnderline, me->CurrentA);
	    if (me->inBoldH == FALSE)
		HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
	    HTML_put_string(me, (id_string ? id_string : href));
	    FREE(href);
	    if (me->inBoldH == FALSE)
		HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
	    HText_endAnchor(me->text, 0);
	    LYEnsureSingleSpace(me);
	    CAN_JUSTIFY_POP;
	} else {
	    CHECK_ID(HTML_FRAME_ID);
	}
	FREE(id_string);
	break;

    case HTML_NOFRAMES:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	break;

    case HTML_IFRAME:
	if (present && present[HTML_IFRAME_NAME] &&
	    non_empty(value[HTML_IFRAME_NAME])) {
	    StrAllocCopy(id_string, value[HTML_IFRAME_NAME]);
	    TRANSLATE_AND_UNESCAPE_ENTITIES(&id_string, TRUE, FALSE);
	    LYTrimHead(id_string);
	    LYTrimTail(id_string);
	}
	if (present && present[HTML_IFRAME_SRC] &&
	    non_empty(value[HTML_IFRAME_SRC])) {
	    StrAllocCopy(href, value[HTML_IFRAME_SRC]);
	    LYLegitimizeHREF(me, &href, TRUE, TRUE);

	    if (me->inA)
		HTML_end_element(me, HTML_A, include);

	    me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
						     NULL,	 
						     href,	 
						     (HTLinkType *) 0);		 
	    LYEnsureDoubleSpace(me);
	    CAN_JUSTIFY_PUSH_F
		LYResetParagraphAlignment(me);
	    if (me->inUnderline == FALSE)
		HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	    HTML_put_string(me, "IFRAME:");
	    if (me->inUnderline == FALSE)
		HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	    HTML_put_character(me, ' ');

	    me->in_word = NO;
	    CHECK_ID(HTML_IFRAME_ID);
	    HText_beginAnchor(me->text, me->inUnderline, me->CurrentA);
	    if (me->inBoldH == FALSE)
		HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
	    HTML_put_string(me, (id_string ? id_string : href));
	    FREE(href);
	    if (me->inBoldH == FALSE)
		HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
	    HText_endAnchor(me->text, 0);
	    LYEnsureSingleSpace(me);
	    CAN_JUSTIFY_POP;
	} else {
	    CHECK_ID(HTML_IFRAME_ID);
	}
	FREE(id_string);
	break;

    case HTML_BANNER:
    case HTML_MARQUEE:
	change_paragraph_style(me, styles[HTML_BANNER]);
	UPDATE_STYLE;
	if (me->sp->tag_number == (int) ElementNumber)
	    LYEnsureDoubleSpace(me);
	 
	if ((!HText_hasToolbar(me->text) &&
	     HText_getLines(me->text) < (display_lines / 2)) &&
	    (ID_A = HTAnchor_findChildAndLink(me->node_anchor,	 
					      LYToolbarName,	 
					      NULL,	 
					      (HTLinkType *) 0))) {	 
	    HText_beginAnchor(me->text, me->inUnderline, ID_A);
	    HText_endAnchor(me->text, 0);
	    HText_setToolbar(me->text);
	}
	CHECK_ID(HTML_GEN_ID);
	break;

    case HTML_CENTER:
    case HTML_DIV:
	if (me->Division_Level < (MAX_NESTING - 1)) {
	    me->Division_Level++;
	} else {
	    CTRACE((tfp,
		    "HTML: ****** Maximum nesting of %d divisions exceeded!\n",
		    MAX_NESTING));
	}
	if (me->inP)
	    LYEnsureSingleSpace(me);	 
	if (ElementNumber == HTML_CENTER) {
	    me->DivisionAlignments[me->Division_Level] = HT_CENTER;
	    change_paragraph_style(me, styles[HTML_DCENTER]);
	    UPDATE_STYLE;
	    me->current_default_alignment = styles[HTML_DCENTER]->alignment;
	} else if (me->List_Nesting_Level >= 0 &&
		   !(present && present[HTML_DIV_ALIGN] &&
		     value[HTML_DIV_ALIGN] &&
		     (!strcasecomp(value[HTML_DIV_ALIGN], "center") ||
		      !strcasecomp(value[HTML_DIV_ALIGN], "right")))) {
	    if (present && present[HTML_DIV_ALIGN])
		me->current_default_alignment = HT_LEFT;
	    else if (me->Division_Level == 0)
		me->current_default_alignment = HT_LEFT;
	    else if (me->sp[0].tag_number == HTML_UL ||
		     me->sp[0].tag_number == HTML_OL ||
		     me->sp[0].tag_number == HTML_MENU ||
		     me->sp[0].tag_number == HTML_DIR ||
		     me->sp[0].tag_number == HTML_LI ||
		     me->sp[0].tag_number == HTML_LH ||
		     me->sp[0].tag_number == HTML_DD)
		me->current_default_alignment = HT_LEFT;
	    LYHandlePlike(me, present, value, include, HTML_DIV_ALIGN, TRUE);
	    me->DivisionAlignments[me->Division_Level] = (short)
		me->current_default_alignment;
	} else if (present && present[HTML_DIV_ALIGN] &&
		   non_empty(value[HTML_DIV_ALIGN])) {
	    if (!strcasecomp(value[HTML_DIV_ALIGN], "center")) {
		me->DivisionAlignments[me->Division_Level] = HT_CENTER;
		change_paragraph_style(me, styles[HTML_DCENTER]);
		UPDATE_STYLE;
		me->current_default_alignment = styles[HTML_DCENTER]->alignment;
	    } else if (!strcasecomp(value[HTML_DIV_ALIGN], "right")) {
		me->DivisionAlignments[me->Division_Level] = HT_RIGHT;
		change_paragraph_style(me, styles[HTML_DRIGHT]);
		UPDATE_STYLE;
		me->current_default_alignment = styles[HTML_DRIGHT]->alignment;
	    } else {
		me->DivisionAlignments[me->Division_Level] = HT_LEFT;
		change_paragraph_style(me, styles[HTML_DLEFT]);
		UPDATE_STYLE;
		me->current_default_alignment = styles[HTML_DLEFT]->alignment;
	    }
	} else {
	    me->DivisionAlignments[me->Division_Level] = HT_LEFT;
	    change_paragraph_style(me, styles[HTML_DLEFT]);
	    UPDATE_STYLE;
	    me->current_default_alignment = styles[HTML_DLEFT]->alignment;
	}
	CHECK_ID(HTML_DIV_ID);
	break;

    case HTML_H1:
    case HTML_H2:
    case HTML_H3:
    case HTML_H4:
    case HTML_H5:
    case HTML_H6:
	 

	 
	if ((me->List_Nesting_Level >= 0) &&
	    (me->sp[0].tag_number == HTML_UL ||
	     me->sp[0].tag_number == HTML_OL ||
	     me->sp[0].tag_number == HTML_MENU ||
	     me->sp[0].tag_number == HTML_DIR ||
	     me->sp[0].tag_number == HTML_LI)) {
	    if (HTML_dtd.tags[HTML_LH].contents == SGML_EMPTY) {
		ElementNumber = HTML_LH;
	    } else {
		me->new_style = me->sp[0].style;
		ElementNumber = (HTMLElement) me->sp[0].tag_number;
		UPDATE_STYLE;
	    }
	     
	    if (!(me->inP == FALSE && me->in_word == NO)) {
		HText_appendParagraph(me->text);
		HTML_put_character(me, HT_NON_BREAK_SPACE);
		HText_setLastChar(me->text, ' ');
		me->in_word = NO;
		me->inP = FALSE;
	    }
	    CHECK_ID(HTML_H_ID);
	    break;
	}

	if (present && present[HTML_H_ALIGN] &&
	    non_empty(value[HTML_H_ALIGN])) {
	    if (!strcasecomp(value[HTML_H_ALIGN], "center"))
		change_paragraph_style(me, styles[HTML_HCENTER]);
	    else if (!strcasecomp(value[HTML_H_ALIGN], "right"))
		change_paragraph_style(me, styles[HTML_HRIGHT]);
	    else if (!strcasecomp(value[HTML_H_ALIGN], "left") ||
		     !strcasecomp(value[HTML_H_ALIGN], "justify"))
		change_paragraph_style(me, styles[HTML_HLEFT]);
	    else
		change_paragraph_style(me, styles[ElementNumber]);
	} else if (me->Division_Level >= 0) {
	    if (me->DivisionAlignments[me->Division_Level] == HT_CENTER) {
		change_paragraph_style(me, styles[HTML_HCENTER]);
	    } else if (me->DivisionAlignments[me->Division_Level] == HT_LEFT) {
		change_paragraph_style(me, styles[HTML_HLEFT]);
	    } else if (me->DivisionAlignments[me->Division_Level] == HT_RIGHT) {
		change_paragraph_style(me, styles[HTML_HRIGHT]);
	    }
	} else {
	    change_paragraph_style(me, styles[ElementNumber]);
	}
	UPDATE_STYLE;
	CHECK_ID(HTML_H_ID);

	if ((bold_headers == TRUE ||
	     (ElementNumber == HTML_H1 && bold_H1 == TRUE)) &&
	    (styles[ElementNumber]->font & HT_BOLD)) {
	    if (me->inBoldA == FALSE && me->inBoldH == FALSE) {
		HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
	    }
	    me->inBoldH = TRUE;
	}
	break;

    case HTML_P:
	LYHandlePlike(me, present, value, include, HTML_P_ALIGN, TRUE);
	CHECK_ID(HTML_P_ID);
	break;

    case HTML_BR:
	UPDATE_STYLE;
	CHECK_ID(HTML_GEN_ID);
	 
	if ((LYCollapseBRs == FALSE &&
	     (!LYtrimBlankLines ||
	      !HText_PreviousLineEmpty(me->text, FALSE))) ||
	    !HText_LastLineEmpty(me->text, FALSE)) {
	    HText_setLastChar(me->text, ' ');	 
	    HText_appendCharacter(me->text, '\r');
	}
	me->in_word = NO;
	me->inP = FALSE;
	break;

    case HTML_WBR:
	UPDATE_STYLE;
	CHECK_ID(HTML_GEN_ID);
	HText_setBreakPoint(me->text);
	break;

    case HTML_HY:
    case HTML_SHY:
	UPDATE_STYLE;
	CHECK_ID(HTML_GEN_ID);
	HText_appendCharacter(me->text, LY_SOFT_HYPHEN);
	break;

    case HTML_HR:
	{
	    int width;

	     
	    UPDATE_STYLE;
	    if (!HText_LastLineEmpty(me->text, FALSE)) {
		HText_setLastChar(me->text, ' ');	 
		HText_appendCharacter(me->text, '\r');
	    } else if (HText_PreviousLineEmpty(me->text, FALSE)) {
		HText_RemovePreviousLine(me->text);
	    }
	    me->in_word = NO;
	    me->inP = FALSE;

	     
	    CHECK_ID(HTML_HR_ID);

	     
	    if (present && present[HTML_HR_ALIGN] && value[HTML_HR_ALIGN]) {
		if (!strcasecomp(value[HTML_HR_ALIGN], "right")) {
		    me->sp->style->alignment = HT_RIGHT;
		} else if (!strcasecomp(value[HTML_HR_ALIGN], "left")) {
		    me->sp->style->alignment = HT_LEFT;
		} else {
		    me->sp->style->alignment = HT_CENTER;
		}
	    } else {
		me->sp->style->alignment = HT_CENTER;
	    }
	    width = LYcolLimit -
		me->new_style->leftIndent - me->new_style->rightIndent;
	    if (present && present[HTML_HR_WIDTH] && value[HTML_HR_WIDTH] &&
		isdigit(UCH(*value[HTML_HR_WIDTH])) &&
		value[HTML_HR_WIDTH][strlen(value[HTML_HR_WIDTH]) - 1] == '%') {
		char *percent = NULL;
		int Percent, Width;

		StrAllocCopy(percent, value[HTML_HR_WIDTH]);
		percent[strlen(percent) - 1] = '\0';
		Percent = atoi(percent);
		if (Percent > 100 || Percent < 1)
		    width -= 5;
		else {
		    Width = (width * Percent) / 100;
		    if (Width < 1)
			width = 1;
		    else
			width = Width;
		}
		FREE(percent);
	    } else {
		width -= 5;
	    }
	    for (i = 0; i < width; i++)
		HTML_put_character(me, '_');
	    HText_appendCharacter(me->text, '\r');
	    me->in_word = NO;
	    me->inP = FALSE;

	     
	    if (me->List_Nesting_Level < 0 &&
		me->Division_Level >= 0) {
		me->sp->style->alignment =
		    me->DivisionAlignments[me->Division_Level];
	    } else if (me->sp->style->id == ST_HeadingCenter ||
		       me->sp->style->id == ST_Heading1) {
		me->sp->style->alignment = HT_CENTER;
	    } else if (me->sp->style->id == ST_HeadingRight) {
		me->sp->style->alignment = HT_RIGHT;
	    } else {
		me->sp->style->alignment = HT_LEFT;
	    }

	     
	    if (me->List_Nesting_Level >= 0 ||
		me->sp[0].tag_number == HTML_ADDRESS) {
		HText_setLastChar(me->text, ' ');	 
		HText_appendCharacter(me->text, '\r');
	    } else {
		HText_appendParagraph(me->text);
	    }
	}
	break;

    case HTML_TAB:
	if (!present) {		 
	    CTRACE((tfp, "HTML: TAB tag has no attributes.  Ignored.\n"));
	    break;
	}
	 
	HText_cancelStbl(me->text);
	UPDATE_STYLE;

	CANT_JUSTIFY_THIS_LINE;
	if (present[HTML_TAB_ALIGN] && value[HTML_TAB_ALIGN] &&
	    (strcasecomp(value[HTML_TAB_ALIGN], "left") ||
	     !(present[HTML_TAB_TO] || present[HTML_TAB_INDENT]))) {
	     
	    HTML_put_character(me, ' ');
	    CTRACE((tfp,
		    "HTML: ALIGN not 'left'.  Using space instead of TAB.\n"));

	} else if (!LYoverride_default_alignment(me) &&
		   me->current_default_alignment != HT_LEFT) {
	     
	    HTML_put_character(me, ' ');
	    CTRACE((tfp, "HTML: Not HT_LEFT.  Using space instead of TAB.\n"));

	} else if ((present[HTML_TAB_TO] &&
		    non_empty(value[HTML_TAB_TO])) ||
		   (present[HTML_TAB_INDENT] &&
		    value[HTML_TAB_INDENT] &&
		    isdigit(UCH(*value[HTML_TAB_INDENT])))) {
	    int column, target = -1;
	    int enval = 2;

	    column = HText_getCurrentColumn(me->text);
	    if (present[HTML_TAB_TO] &&
		non_empty(value[HTML_TAB_TO])) {
		 
		StrAllocCopy(temp, value[HTML_TAB_TO]);
		TRANSLATE_AND_UNESCAPE_TO_STD(&temp);
		if (*temp) {
		    target = HText_getTabIDColumn(me->text, temp);
		}
	    } else if (isEmpty(temp) && present[HTML_TAB_INDENT] &&
		       value[HTML_TAB_INDENT] &&
		       isdigit(UCH(*value[HTML_TAB_INDENT]))) {
		 
		target =
		    (int) (((1.0 * atoi(value[HTML_TAB_INDENT])) / enval) + (0.5));
	    }
	    FREE(temp);
	     

	    if (target < column ||
		target > HText_getMaximumColumn(me->text)) {
		HTML_put_character(me, ' ');
		CTRACE((tfp,
			"HTML: Column out of bounds.  Using space instead of TAB.\n"));
	    } else {
		for (i = column; i < target; i++)
		    HText_appendCharacter(me->text, ' ');
		HText_setLastChar(me->text, ' ');	 
	    }
	}
	me->in_word = NO;

	 
	if (present[HTML_TAB_ID] &&
	    non_empty(value[HTML_TAB_ID])) {
	    StrAllocCopy(temp, value[HTML_TAB_ID]);
	    TRANSLATE_AND_UNESCAPE_TO_STD(&temp);
	    if (*temp)
		HText_setTabID(me->text, temp);
	    FREE(temp);
	}
	break;

    case HTML_BASEFONT:
	break;

    case HTML_FONT:

	 
#ifdef NOTUSED_FOTEMODS
	if (me->inFONT == TRUE)
	    HTML_end_element(me, HTML_FONT, &include);
#endif  

	 
	me->inFONT = TRUE;
	break;

    case HTML_B:		 
    case HTML_BLINK:
    case HTML_I:
    case HTML_U:

    case HTML_CITE:		 
    case HTML_EM:
    case HTML_STRONG:
	UPDATE_STYLE;
	me->Underline_Level++;
	CHECK_ID(HTML_GEN_ID);
	 
	if (me->inBoldA == TRUE || me->inBoldH == TRUE) {
	    CTRACE((tfp, "Underline Level is %d\n", me->Underline_Level));
	    break;
	}
	if (me->inUnderline == FALSE) {
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	    me->inUnderline = TRUE;
	    CTRACE((tfp, "Beginning underline\n"));
	} else {
	    CTRACE((tfp, "Underline Level is %d\n", me->Underline_Level));
	}
	break;

    case HTML_ABBR:		 
    case HTML_ACRONYM:
    case HTML_AU:
    case HTML_AUTHOR:
    case HTML_BIG:
    case HTML_CODE:
    case HTML_DFN:
    case HTML_KBD:
    case HTML_SAMP:
    case HTML_SMALL:
    case HTML_TT:
    case HTML_VAR:
	CHECK_ID(HTML_GEN_ID);
	break;			 

    case HTML_SUP:
	HText_appendCharacter(me->text, '^');
	CHECK_ID(HTML_GEN_ID);
	break;

    case HTML_SUB:
	HText_appendCharacter(me->text, '[');
	CHECK_ID(HTML_GEN_ID);
	break;

    case HTML_DEL:
    case HTML_S:
    case HTML_STRIKE:
	CHECK_ID(HTML_GEN_ID);
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	HTML_put_string(me, "[DEL:");
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	HTML_put_character(me, ' ');
	me->in_word = NO;
	break;

    case HTML_INS:
	CHECK_ID(HTML_GEN_ID);
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	HTML_put_string(me, "[INS:");
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	HTML_put_character(me, ' ');
	me->in_word = NO;
	break;

    case HTML_Q:
	CHECK_ID(HTML_GEN_ID);
	 
	if (!(me->Quote_Level & 1))
	    HTML_put_character(me, '"');
	else
	    HTML_put_character(me, '`');
	me->Quote_Level++;
	break;

    case HTML_PRE:		 
	 
	me->inPRE = FALSE;
	 
    case HTML_LISTING:		 
	 
    case HTML_XMP:
	 
    case HTML_PLAINTEXT:
	change_paragraph_style(me, styles[ElementNumber]);
	UPDATE_STYLE;
	CHECK_ID(HTML_GEN_ID);
	if (me->comment_end)
	    HText_appendText(me->text, me->comment_end);
	break;

    case HTML_BLOCKQUOTE:
    case HTML_BQ:
	change_paragraph_style(me, styles[ElementNumber]);
	UPDATE_STYLE;
	if (me->sp->tag_number == (int) ElementNumber)
	    LYEnsureDoubleSpace(me);
	CHECK_ID(HTML_BQ_ID);
	break;

    case HTML_NOTE:
	change_paragraph_style(me, styles[ElementNumber]);
	UPDATE_STYLE;
	if (me->sp->tag_number == (int) ElementNumber)
	    LYEnsureDoubleSpace(me);
	CHECK_ID(HTML_NOTE_ID);
	{
	    char *note = NULL;

	     
	    if (present && present[HTML_NOTE_CLASS] &&
		value[HTML_NOTE_CLASS] &&
		(!strcasecomp(value[HTML_NOTE_CLASS], "CAUTION") ||
		 !strcasecomp(value[HTML_NOTE_CLASS], "WARNING"))) {
		StrAllocCopy(note, value[HTML_NOTE_CLASS]);
		LYUpperCase(note);
		StrAllocCat(note, ":");
	    } else if (present && present[HTML_NOTE_ROLE] &&
		       value[HTML_NOTE_ROLE] &&
		       (!strcasecomp(value[HTML_NOTE_ROLE], "CAUTION") ||
			!strcasecomp(value[HTML_NOTE_ROLE], "WARNING"))) {
		StrAllocCopy(note, value[HTML_NOTE_ROLE]);
		LYUpperCase(note);
		StrAllocCat(note, ":");
	    } else {
		StrAllocCopy(note, "NOTE:");
	    }
	    if (me->inUnderline == FALSE)
		HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	    HTML_put_string(me, note);
	    if (me->inUnderline == FALSE)
		HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	    HTML_put_character(me, ' ');
	    CAN_JUSTIFY_START;
	    FREE(note);
	}
	CAN_JUSTIFY_START;
	me->inLABEL = TRUE;
	me->in_word = NO;
	me->inP = FALSE;
	break;

    case HTML_ADDRESS:
	if (me->List_Nesting_Level < 0) {
	    change_paragraph_style(me, styles[ElementNumber]);
	    UPDATE_STYLE;
	    if (me->sp->tag_number == (int) ElementNumber)
		LYEnsureDoubleSpace(me);
	} else {
	    LYHandlePlike(me, present, value, include, -1, TRUE);
	}
	CHECK_ID(HTML_ADDRESS_ID);
	break;

    case HTML_DL:
	me->List_Nesting_Level++;	 
	if (me->List_Nesting_Level <= 0) {
	    change_paragraph_style(me, present && present[HTML_DL_COMPACT]
				   ? styles[HTML_DLC] : styles[HTML_DL]);

	} else if (me->List_Nesting_Level >= 6) {
	    change_paragraph_style(me, present && present[HTML_DL_COMPACT]
				   ? styles[HTML_DLC6] : styles[HTML_DL6]);

	} else {
	    change_paragraph_style(me, present && present[HTML_DL_COMPACT]
				   ? styles[(HTML_DLC1 - 1) + me->List_Nesting_Level]
				   : styles[(HTML_DL1 - 1) + me->List_Nesting_Level]);
	}
	UPDATE_STYLE;		 
	CHECK_ID(HTML_DL_ID);

	break;

    case HTML_DLC:
	me->List_Nesting_Level++;	 
	if (me->List_Nesting_Level <= 0) {
	    change_paragraph_style(me, styles[HTML_DLC]);

	} else if (me->List_Nesting_Level >= 6) {
	    change_paragraph_style(me, styles[HTML_DLC6]);

	} else {
	    change_paragraph_style(me,
				   styles[(HTML_DLC1 - 1) + me->List_Nesting_Level]);
	}
	UPDATE_STYLE;		 
	CHECK_ID(HTML_DL_ID);
	break;

    case HTML_DT:
	CHECK_ID(HTML_GEN_ID);
	if (!me->style_change) {
	    BOOL in_line_1 = HText_inLineOne(me->text);
	    HTCoord saved_spaceBefore = me->sp->style->spaceBefore;
	    HTCoord saved_spaceAfter = me->sp->style->spaceAfter;

	     
	    if (in_line_1) {
		me->sp->style->spaceBefore = 0;		 
		me->sp->style->spaceAfter = 0;	 
	    }
	    HText_appendParagraph(me->text);
	    me->sp->style->spaceBefore = saved_spaceBefore;	 
	    me->sp->style->spaceAfter = saved_spaceAfter;	 
	    me->in_word = NO;
	    me->sp->style->alignment = HT_LEFT;
	}
	me->inP = FALSE;
	break;

    case HTML_DD:
	CHECK_ID(HTML_GEN_ID);
	HText_setLastChar(me->text, ' ');	 
	if (!me->style_change) {
	    if (!HText_LastLineEmpty(me->text, FALSE)) {
		HText_appendCharacter(me->text, '\r');
	    } else {
		HText_NegateLineOne(me->text);
	    }
	} else {
	    UPDATE_STYLE;
	    HText_appendCharacter(me->text, '\t');
	}
	me->sp->style->alignment = HT_LEFT;
	me->in_word = NO;
	me->inP = FALSE;
	break;

    case HTML_OL:
	 
	me->OL_Type[(me->List_Nesting_Level < 11 ?
		     me->List_Nesting_Level + 1 : 11)] = '1';

	 
	if (present && (present[HTML_OL_SEQNUM] || present[HTML_OL_START])) {
	    int seqnum;

	     
	    if (present[HTML_OL_SEQNUM] &&
		non_empty(value[HTML_OL_SEQNUM])) {
		seqnum = atoi(value[HTML_OL_SEQNUM]);
	    } else if (present[HTML_OL_START] &&
		       non_empty(value[HTML_OL_START])) {
		seqnum = atoi(value[HTML_OL_START]);
	    } else {
		seqnum = 1;
	    }

	     
	    if (present[HTML_OL_TYPE] && value[HTML_OL_TYPE]) {
		if (*value[HTML_OL_TYPE] == 'A') {
		    me->OL_Type[(me->List_Nesting_Level < 11 ?
				 me->List_Nesting_Level + 1 : 11)] = 'A';
		    if (seqnum < 1)
			seqnum = 1;
		} else if (*value[HTML_OL_TYPE] == 'a') {
		    me->OL_Type[(me->List_Nesting_Level < 11 ?
				 me->List_Nesting_Level + 1 : 11)] = 'a';
		    if (seqnum < 1)
			seqnum = 1;
		} else if (*value[HTML_OL_TYPE] == 'I') {
		    me->OL_Type[(me->List_Nesting_Level < 11 ?
				 me->List_Nesting_Level + 1 : 11)] = 'I';
		    if (seqnum < 1)
			seqnum = 1;
		} else if (*value[HTML_OL_TYPE] == 'i') {
		    me->OL_Type[(me->List_Nesting_Level < 11 ?
				 me->List_Nesting_Level + 1 : 11)] = 'i';
		    if (seqnum < 1)
			seqnum = 1;
		} else {
		    if (seqnum <= OL_VOID)
			seqnum = OL_VOID + 1;
		}
	    } else if (seqnum <= OL_VOID) {
		seqnum = OL_VOID + 1;
	    }

	    me->OL_Counter[(me->List_Nesting_Level < 11 ?
			    me->List_Nesting_Level + 1 : 11)] = seqnum;

	} else if (present && present[HTML_OL_CONTINUE]) {
	    me->OL_Counter[me->List_Nesting_Level < 11 ?
			   me->List_Nesting_Level + 1 : 11] = OL_CONTINUE;

	} else {
	    me->OL_Counter[(me->List_Nesting_Level < 11 ?
			    me->List_Nesting_Level + 1 : 11)] = 1;
	    if (present && present[HTML_OL_TYPE] && value[HTML_OL_TYPE]) {
		if (*value[HTML_OL_TYPE] == 'A') {
		    me->OL_Type[(me->List_Nesting_Level < 11 ?
				 me->List_Nesting_Level + 1 : 11)] = 'A';
		} else if (*value[HTML_OL_TYPE] == 'a') {
		    me->OL_Type[(me->List_Nesting_Level < 11 ?
				 me->List_Nesting_Level + 1 : 11)] = 'a';
		} else if (*value[HTML_OL_TYPE] == 'I') {
		    me->OL_Type[(me->List_Nesting_Level < 11 ?
				 me->List_Nesting_Level + 1 : 11)] = 'I';
		} else if (*value[HTML_OL_TYPE] == 'i') {
		    me->OL_Type[(me->List_Nesting_Level < 11 ?
				 me->List_Nesting_Level + 1 : 11)] = 'i';
		}
	    }
	}
	me->List_Nesting_Level++;

	if (me->List_Nesting_Level <= 0) {
	    change_paragraph_style(me, styles[ElementNumber]);

	} else if (me->List_Nesting_Level >= 6) {
	    change_paragraph_style(me, styles[HTML_OL6]);

	} else {
	    change_paragraph_style(me,
				   styles[HTML_OL1 + me->List_Nesting_Level - 1]);
	}
	UPDATE_STYLE;		 
	CHECK_ID(HTML_OL_ID);
	break;

    case HTML_UL:
	me->List_Nesting_Level++;

	if (me->List_Nesting_Level <= 0) {
	    if (!(present && present[HTML_UL_PLAIN]) &&
		!(present && present[HTML_UL_TYPE] &&
		  value[HTML_UL_TYPE] &&
		  0 == strcasecomp(value[HTML_UL_TYPE], "PLAIN"))) {
		change_paragraph_style(me, styles[ElementNumber]);
	    } else {
		change_paragraph_style(me, styles[HTML_DIR]);
		ElementNumber = HTML_DIR;
	    }

	} else if (me->List_Nesting_Level >= 6) {
	    if (!(present && present[HTML_UL_PLAIN]) &&
		!(present && present[HTML_UL_TYPE] &&
		  value[HTML_UL_TYPE] &&
		  0 == strcasecomp(value[HTML_UL_TYPE], "PLAIN"))) {
		change_paragraph_style(me, styles[HTML_OL6]);
	    } else {
		change_paragraph_style(me, styles[HTML_MENU6]);
		ElementNumber = HTML_DIR;
	    }

	} else {
	    if (!(present && present[HTML_UL_PLAIN]) &&
		!(present && present[HTML_UL_TYPE] &&
		  value[HTML_UL_TYPE] &&
		  0 == strcasecomp(value[HTML_UL_TYPE], "PLAIN"))) {
		change_paragraph_style(me,
				       styles[HTML_OL1 + me->List_Nesting_Level
					      - 1]);
	    } else {
		change_paragraph_style(me,
				       styles[HTML_MENU1 + me->List_Nesting_Level
					      - 1]);
		ElementNumber = HTML_DIR;
	    }
	}
	UPDATE_STYLE;		 
	CHECK_ID(HTML_UL_ID);
	break;

    case HTML_MENU:
    case HTML_DIR:
	me->List_Nesting_Level++;

	if (me->List_Nesting_Level <= 0) {
	    change_paragraph_style(me, styles[ElementNumber]);

	} else if (me->List_Nesting_Level >= 6) {
	    change_paragraph_style(me, styles[HTML_MENU6]);

	} else {
	    change_paragraph_style(me,
				   styles[HTML_MENU1 + me->List_Nesting_Level
					  - 1]);
	}
	UPDATE_STYLE;		 
	CHECK_ID(HTML_UL_ID);
	break;

    case HTML_LH:
	UPDATE_STYLE;		 
	HText_appendParagraph(me->text);
	CHECK_ID(HTML_GEN_ID);
	HTML_put_character(me, HT_NON_BREAK_SPACE);
	HText_setLastChar(me->text, ' ');
	me->in_word = NO;
	me->inP = FALSE;
	break;

    case HTML_LI:
	UPDATE_STYLE;		 
	HText_appendParagraph(me->text);
	me->sp->style->alignment = HT_LEFT;
	CHECK_ID(HTML_LI_ID);
	{
	    int surrounding_tag_number = me->sp[0].tag_number;

	     
	    if (surrounding_tag_number == HTML_LI &&
		me->sp < (me->stack + MAX_NESTING - 1))
		surrounding_tag_number = me->sp[1].tag_number;
	    if (surrounding_tag_number == HTML_OL) {
		char number_string[20];
		int counter, seqnum;
		char seqtype;

		counter = me->List_Nesting_Level < 11 ?
		    me->List_Nesting_Level : 11;
		if (present && present[HTML_LI_TYPE] && value[HTML_LI_TYPE]) {
		    if (*value[HTML_LI_TYPE] == '1') {
			me->OL_Type[counter] = '1';
		    } else if (*value[HTML_LI_TYPE] == 'A') {
			me->OL_Type[counter] = 'A';
		    } else if (*value[HTML_LI_TYPE] == 'a') {
			me->OL_Type[counter] = 'a';
		    } else if (*value[HTML_LI_TYPE] == 'I') {
			me->OL_Type[counter] = 'I';
		    } else if (*value[HTML_LI_TYPE] == 'i') {
			me->OL_Type[counter] = 'i';
		    }
		}
		if (present && present[HTML_LI_VALUE] &&
		    ((value[HTML_LI_VALUE] != NULL) &&
		     (*value[HTML_LI_VALUE] != '\0')) &&
		    ((isdigit(UCH(*value[HTML_LI_VALUE]))) ||
		     (*value[HTML_LI_VALUE] == '-' &&
		      isdigit(UCH(*(value[HTML_LI_VALUE] + 1)))))) {
		    seqnum = atoi(value[HTML_LI_VALUE]);
		    if (seqnum <= OL_VOID)
			seqnum = OL_VOID + 1;
		    seqtype = me->OL_Type[counter];
		    if (seqtype != '1' && seqnum < 1)
			seqnum = 1;
		    me->OL_Counter[counter] = seqnum + 1;
		} else if (me->OL_Counter[counter] >= OL_VOID) {
		    seqnum = me->OL_Counter[counter]++;
		    seqtype = me->OL_Type[counter];
		    if (seqtype != '1' && seqnum < 1) {
			seqnum = 1;
			me->OL_Counter[counter] = seqnum + 1;
		    }
		} else {
		    seqnum = me->Last_OL_Count + 1;
		    seqtype = me->Last_OL_Type;
		    for (i = (counter - 1); i >= 0; i--) {
			if (me->OL_Counter[i] > OL_VOID) {
			    seqnum = me->OL_Counter[i]++;
			    seqtype = me->OL_Type[i];
			    i = 0;
			}
		    }
		}
		if (seqtype == 'A') {
		    strcpy(number_string, LYUppercaseA_OL_String(seqnum));
		} else if (seqtype == 'a') {
		    strcpy(number_string, LYLowercaseA_OL_String(seqnum));
		} else if (seqtype == 'I') {
		    strcpy(number_string, LYUppercaseI_OL_String(seqnum));
		} else if (seqtype == 'i') {
		    strcpy(number_string, LYLowercaseI_OL_String(seqnum));
		} else {
		    sprintf(number_string, "%2d.", seqnum);
		}
		me->Last_OL_Count = seqnum;
		me->Last_OL_Type = seqtype;
		 
		for (i = 0; number_string[i] != '\0'; i++)
		    if (number_string[i] == ' ')
			HTML_put_character(me, HT_NON_BREAK_SPACE);
		    else
			HTML_put_character(me, number_string[i]);

		 
		HTML_put_character(me, HT_NON_BREAK_SPACE);
		HText_setLastChar(me->text, ' ');
	    } else if (surrounding_tag_number == HTML_UL) {
		 
		HTML_put_character(me, HT_NON_BREAK_SPACE);
		HTML_put_character(me, HT_NON_BREAK_SPACE);
		switch (me->List_Nesting_Level % 7) {
		case 0:
		    HTML_put_character(me, '*');
		    break;
		case 1:
		    HTML_put_character(me, '+');
		    break;
		case 2:
		    HTML_put_character(me, 'o');
		    break;
		case 3:
		    HTML_put_character(me, '#');
		    break;
		case 4:
		    HTML_put_character(me, '@');
		    break;
		case 5:
		    HTML_put_character(me, '-');
		    break;
		case 6:
		    HTML_put_character(me, '=');
		    break;

		}
		 
		HTML_put_character(me, HT_NON_BREAK_SPACE);
		HText_setLastChar(me->text, ' ');
	    } else {
		 
		HTML_put_character(me, HT_NON_BREAK_SPACE);
		HTML_put_character(me, HT_NON_BREAK_SPACE);
		HText_setLastChar(me->text, ' ');
	    }
	}
	CAN_JUSTIFY_START;
	me->in_word = NO;
	me->inP = FALSE;
	break;

    case HTML_SPAN:
	CHECK_ID(HTML_GEN_ID);
	 
	break;

    case HTML_BDO:
	CHECK_ID(HTML_GEN_ID);
	 
	break;

    case HTML_SPOT:
	CHECK_ID(HTML_GEN_ID);
	break;

    case HTML_FN:
	change_paragraph_style(me, styles[ElementNumber]);
	UPDATE_STYLE;
	if (me->sp->tag_number == (int) ElementNumber)
	    LYEnsureDoubleSpace(me);
	CHECK_ID(HTML_GEN_ID);
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	HTML_put_string(me, "FOOTNOTE:");
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	HTML_put_character(me, ' ');
	CAN_JUSTIFY_START
	    me->inLABEL = TRUE;
	me->in_word = NO;
	me->inP = FALSE;
	break;

    case HTML_A:
	 
	if (me->map_address && present && present[HTML_A_COORDS])
	    LYStartArea(me,
			present[HTML_A_HREF] ? value[HTML_A_HREF] : NULL,
			NULL,
			present[HTML_A_TITLE] ? value[HTML_A_TITLE] : NULL,
			tag_charset);
	if (LYMapsOnly) {
	    return HT_OK;
	}
	 
	if (me->inA) {
	    SET_SKIP_STACK(HTML_A);
	    HTML_end_element(me, HTML_A, include);
	}
	 
	me->inA = TRUE;

	 
	if (present && present[HTML_A_ID] &&
	    non_empty(value[HTML_A_ID])) {
	    StrAllocCopy(id_string, value[HTML_A_ID]);
	} else if (present && present[HTML_A_NAME] &&
		   non_empty(value[HTML_A_NAME])) {
	    StrAllocCopy(id_string, value[HTML_A_NAME]);
	}
	if (id_string)
	    TRANSLATE_AND_UNESCAPE_TO_STD(&id_string);

	 
	if (present && present[HTML_A_HREF]) {
	     
	    me->inBoldA = TRUE;

	    if (isEmpty(value[HTML_A_HREF]))
		StrAllocCopy(href, "#");
	    else
		StrAllocCopy(href, value[HTML_A_HREF]);
	    CHECK_FOR_INTERN(intern_flag, href);	 

	    if (intern_flag) {  
		TRANSLATE_AND_UNESCAPE_TO_STD(&href);

	    } else {
		url_type = LYLegitimizeHREF(me, &href, TRUE, TRUE);

		 
		if (!url_type && !StrNCmp(href, "/foo/..", 7) &&
		    (isFTP_URL(me->node_anchor->address) ||
		     isFILE_URL(me->node_anchor->address))) {
		    for (i = 0; (href[i] = href[i + 7]) != 0; i++) ;
		}
	    }

	    if (present[HTML_A_ISMAP])	 
		intern_flag = FALSE;
	} else {
	    if (bold_name_anchors == TRUE) {
		me->inBoldA = TRUE;
	    }
	}

	if (present && present[HTML_A_TYPE] && value[HTML_A_TYPE]) {
	    StrAllocCopy(temp, value[HTML_A_TYPE]);
	    if (!intern_flag &&
		!strcasecomp(value[HTML_A_TYPE], HTAtom_name(HTInternalLink)) &&
		!LYIsUIPage3(me->node_anchor->address, UIP_LIST_PAGE, 0) &&
		!LYIsUIPage3(me->node_anchor->address, UIP_ADDRLIST_PAGE, 0) &&
		!isLYNXIMGMAP(me->node_anchor->address)) {
		 
		CTRACE((tfp, "HTML: Found invalid HREF=\"%s\" TYPE=\"%s\"!\n",
			href, temp));
		FREE(temp);
	    }
	}

	me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
						 id_string,	 
						 href,	 
						 (temp
						  ? (HTLinkType *)
						  HTAtom_for(temp)
						  : INTERN_LT));	 
	FREE(temp);
	FREE(id_string);

	if (me->CurrentA && present) {
	    if (present[HTML_A_TITLE] &&
		non_empty(value[HTML_A_TITLE])) {
		StrAllocCopy(title, value[HTML_A_TITLE]);
		TRANSLATE_AND_UNESCAPE_ENTITIES(&title, TRUE, FALSE);
		LYTrimHead(title);
		LYTrimTail(title);
		if (*title == '\0') {
		    FREE(title);
		}
	    }
	    if (present[HTML_A_ISMAP])
		dest_ismap = TRUE;
	    if (present[HTML_A_CHARSET] &&
		non_empty(value[HTML_A_CHARSET])) {
		 
		StrAllocCopy(temp, value[HTML_A_CHARSET]);
		TRANSLATE_AND_UNESCAPE_TO_STD(&temp);
		dest_char_set = UCGetLYhndl_byMIME(temp);
		if (dest_char_set < 0) {
		    dest_char_set = UCLYhndl_for_unrec;
		}
	    }
	    if (title != NULL || dest_ismap == TRUE || dest_char_set >= 0) {
		dest = HTAnchor_parent(HTAnchor_followLink(me->CurrentA)
		    );
	    }
	    if (dest && title != NULL && HTAnchor_title(dest) == NULL)
		HTAnchor_setTitle(dest, title);
	    if (dest && dest_ismap)
		dest->isISMAPScript = TRUE;
	     
	    if (dest && dest != me->node_anchor && dest_char_set >= 0) {
		 
		HTAnchor_setUCInfoStage(dest, dest_char_set,
					UCT_STAGE_MIME,
					UCT_SETBY_DEFAULT);
		HTAnchor_setUCInfoStage(dest, dest_char_set,
					UCT_STAGE_PARSER,
					UCT_SETBY_LINK);
	    }
	    FREE(temp);
	    dest = NULL;
	    FREE(title);
	}
	me->CurrentANum = HText_beginAnchor(me->text,
					    me->inUnderline, me->CurrentA);
	if (me->inBoldA == TRUE && me->inBoldH == FALSE)
	    HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
#if defined(NOTUSED_FOTEMODS)
	 
	if (href == NULL && me->inBoldA == FALSE) {
	    SET_SKIP_STACK(HTML_A);
	    HTML_end_element(me, HTML_A, include);
	}
#else
	 
	if (href == NULL && force_empty_hrefless_a) {
	    SET_SKIP_STACK(HTML_A);
	    HTML_end_element(me, HTML_A, include);
	}
#endif
	FREE(href);
	break;

    case HTML_IMG:		 
	 
	if (me->inA && me->CurrentA) {
	    if ((dest = HTAnchor_parent(HTAnchor_followLink(me->CurrentA)
		 )) != NULL) {
		if (dest->isISMAPScript == TRUE) {
		    dest_ismap = TRUE;
		    CTRACE((tfp, "HTML: '%s' is an ISMAP script\n",
			    dest->address));
		} else if (present && present[HTML_IMG_ISMAP]) {
		    dest_ismap = TRUE;
		    dest->isISMAPScript = TRUE;
		    CTRACE((tfp, "HTML: Designating '%s' as an ISMAP script\n",
			    dest->address));
		}
	    }
	}

	intern_flag = FALSE;	 
	 
	if (present && present[HTML_IMG_USEMAP] &&
	    non_empty(value[HTML_IMG_USEMAP])) {
	    StrAllocCopy(map_href, value[HTML_IMG_USEMAP]);
	    CHECK_FOR_INTERN(intern_flag, map_href);
	    (void) LYLegitimizeHREF(me, &map_href, TRUE, TRUE);
	     
	    if (findPoundSelector(map_href) == NULL) {
		FREE(map_href);
	    }
	}

	 
	if (map_href) {
	     
	     
	    Base = ((me->inBASE &&
		     !(*map_href == '#' && LYSeekFragMAPinCur == TRUE))
		    ? me->base_href
		    : me->node_anchor->address);
	    HTParseALL(&map_href, Base);

	     
	    StrAllocCopy(temp, STR_LYNXIMGMAP);
	    StrAllocCat(temp, map_href);
	    StrAllocCopy(map_href, temp);
	    FREE(temp);
	}

	 
	if (LYNoISMAPifUSEMAP && map_href && dest_ismap) {
	    dest_ismap = FALSE;
	    dest = NULL;
	}

	 
	if (present && present[HTML_IMG_TITLE] &&
	    non_empty(value[HTML_IMG_TITLE])) {
	    StrAllocCopy(title, value[HTML_IMG_TITLE]);
	    TRANSLATE_AND_UNESCAPE_ENTITIES(&title, TRUE, FALSE);
	    LYTrimHead(title);
	    LYTrimTail(title);
	    if (*title == '\0') {
		FREE(title);
	    }
	}

	 
	if (((present) &&
	     (present[HTML_IMG_ALT] && value[HTML_IMG_ALT])) &&
	    (!clickable_images ||
	     ((clickable_images || map_href) &&
	      *value[HTML_IMG_ALT] != '\0'))) {
	    StrAllocCopy(alt_string, value[HTML_IMG_ALT]);
	    TRANSLATE_AND_UNESCAPE_ENTITIES(&alt_string,
					    me->UsePlainSpace, me->HiddenValue);
	     
	    if (clickable_images || map_href) {
		LYTrimHead(alt_string);
		LYTrimTail(alt_string);
		if (*alt_string == '\0') {
		    if (map_href) {
			StrAllocCopy(alt_string, (title ? title :
						  (temp = MakeNewMapValue(value,
									  "USEMAP"))));
			FREE(temp);
		    } else if (dest_ismap) {
			StrAllocCopy(alt_string, (title ? title :
						  (temp = MakeNewMapValue(value,
									  "ISMAP"))));
			FREE(temp);

		    } else if (me->inA == TRUE && dest) {
			StrAllocCopy(alt_string, (title ?
						  title :
						  VERBOSE_IMG(value, HTML_IMG_SRC,
							      "[LINK]")));

		    } else {
			StrAllocCopy(alt_string,
				     (title ? title :
				      ((present &&
					present[HTML_IMG_ISOBJECT]) ?
				       "(OBJECT)" :
				       VERBOSE_IMG(value, HTML_IMG_SRC,
						   "[INLINE]"))));
		    }
		}
	    }

	} else if (map_href) {
	    StrAllocCopy(alt_string, (title ? title :
				      (temp = MakeNewMapValue(value, "USEMAP"))));
	    FREE(temp);

	} else if ((dest_ismap == TRUE) ||
		   (me->inA && present && present[HTML_IMG_ISMAP])) {
	    StrAllocCopy(alt_string, (title ? title :
				      (temp = MakeNewMapValue(value, "ISMAP"))));
	    FREE(temp);

	} else if (me->inA == TRUE && dest) {
	    StrAllocCopy(alt_string, (title ?
				      title :
				      VERBOSE_IMG(value, HTML_IMG_SRC,
						  "[LINK]")));

	} else {
	    if (pseudo_inline_alts || clickable_images)
		StrAllocCopy(alt_string, (title ? title :
					  ((present &&
					    present[HTML_IMG_ISOBJECT]) ?
					   "(OBJECT)" :
					   VERBOSE_IMG(value, HTML_IMG_SRC,
						       "[INLINE]"))));
	    else
		StrAllocCopy(alt_string, NonNull(title));
	}
	if (*alt_string == '\0' && map_href) {
	    StrAllocCopy(alt_string, (temp = MakeNewMapValue(value, "USEMAP")));
	    FREE(temp);
	}

	CTRACE((tfp, "HTML IMG: USEMAP=%d ISMAP=%d ANCHOR=%d PARA=%d\n",
		map_href ? 1 : 0,
		(dest_ismap == TRUE) ? 1 : 0,
		me->inA, me->inP));

	 
	if (present && present[HTML_IMG_ID] &&
	    non_empty(value[HTML_IMG_ID])) {
	    StrAllocCopy(id_string, value[HTML_IMG_ID]);
	    TRANSLATE_AND_UNESCAPE_TO_STD(&id_string);
	    if (*id_string == '\0') {
		FREE(id_string);
	    }
	}

	 
	if (clickable_images &&
	    present && present[HTML_IMG_SRC] &&
	    non_empty(value[HTML_IMG_SRC])) {
	    StrAllocCopy(href, value[HTML_IMG_SRC]);
	    LYLegitimizeHREF(me, &href, TRUE, TRUE);

	     
	    if (me->inA) {
		 
		if (map_href) {
		    if (dest_ismap) {
			HTML_put_character(me, ' ');
			me->in_word = NO;
			HTML_put_string(me,
					(temp = MakeNewMapValue(value, "ISMAP")));
			FREE(temp);
		    } else if (dest) {
			HTML_put_character(me, ' ');
			me->in_word = NO;
			HTML_put_string(me, "[LINK]");
		    }
		    if (me->inBoldA == TRUE && me->inBoldH == FALSE) {
			HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		    }
		    me->inBoldA = FALSE;
		    HText_endAnchor(me->text, me->CurrentANum);
		    me->CurrentANum = 0;
		    if (dest_ismap || dest)
			HTML_put_character(me, '-');
		    if (id_string) {
			if ((ID_A = HTAnchor_findChildAndLink(me->node_anchor,	 
							      id_string,	 
							      NULL,	 
							      0)) != NULL) {	 
			    HText_beginAnchor(me->text, me->inUnderline, ID_A);
			    HText_endAnchor(me->text, 0);
			}
		    }
		    me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
							     NULL,	 
							     map_href,	 
							     INTERN_LT);	 
		    if (me->CurrentA && title) {
			if ((dest = HTAnchor_parent(HTAnchor_followLink(me->CurrentA)
			     )) != NULL) {
			    if (!HTAnchor_title(dest))
				HTAnchor_setTitle(dest, title);
			}
		    }
		    me->CurrentANum = HText_beginAnchor(me->text,
							me->inUnderline,
							me->CurrentA);
		    if (me->inBoldA == FALSE && me->inBoldH == FALSE) {
			HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
		    }
		    me->inBoldA = TRUE;
		} else {
		    HTML_put_character(me, ' ');	 
		    me->in_word = NO;
		}
		HTML_put_string(me, alt_string);
		if (me->inBoldA == TRUE && me->inBoldH == FALSE) {
		    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		}
		me->inBoldA = FALSE;
		HText_endAnchor(me->text, me->CurrentANum);
		me->CurrentANum = 0;
		HTML_put_character(me, '-');
		FREE(newtitle);
		StrAllocCopy(alt_string,
			     ((present &&
			       present[HTML_IMG_ISOBJECT]) ?
			      ((map_href || dest_ismap) ?
			       "(IMAGE)" : "(OBJECT)") :
			      VERBOSE_IMG(value, HTML_IMG_SRC, "[IMAGE]")));
		if (id_string && !map_href) {
		    if ((ID_A = HTAnchor_findChildAndLink(me->node_anchor,	 
							  id_string,	 
							  NULL,		 
							  0)) != NULL) {	 
			HText_beginAnchor(me->text, me->inUnderline, ID_A);
			HText_endAnchor(me->text, 0);
		    }
		}
	    } else if (map_href) {
		HTML_put_character(me, ' ');	 
		me->in_word = NO;
		if (id_string) {
		    if ((ID_A = HTAnchor_findChildAndLink(me->node_anchor,	 
							  id_string,	 
							  NULL,		 
							  0)) != NULL) {	 
			HText_beginAnchor(me->text, me->inUnderline, ID_A);
			HText_endAnchor(me->text, 0);
		    }
		}
		me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
							 NULL,	 
							 map_href,	 
							 INTERN_LT);	 
		if (me->CurrentA && title) {
		    if ((dest = HTAnchor_parent(HTAnchor_followLink(me->CurrentA)
			 )) != NULL) {
			if (!HTAnchor_title(dest))
			    HTAnchor_setTitle(dest, title);
		    }
		}
		me->CurrentANum = HText_beginAnchor(me->text,
						    me->inUnderline,
						    me->CurrentA);
		if (me->inBoldA == FALSE && me->inBoldH == FALSE)
		    HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
		me->inBoldA = TRUE;
		HTML_put_string(me, alt_string);
		if (me->inBoldA == TRUE && me->inBoldH == FALSE) {
		    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		}
		me->inBoldA = FALSE;
		HText_endAnchor(me->text, me->CurrentANum);
		me->CurrentANum = 0;
		HTML_put_character(me, '-');
		FREE(newtitle);
		StrAllocCopy(alt_string,
			     ((present &&
			       present[HTML_IMG_ISOBJECT]) ?
			      "(IMAGE)" :
			      VERBOSE_IMG(value, HTML_IMG_SRC, "[IMAGE]")));
	    } else {
		HTML_put_character(me, ' ');	 
		me->in_word = NO;
		if (id_string) {
		    if ((ID_A = HTAnchor_findChildAndLink(me->node_anchor,	 
							  id_string,	 
							  NULL,		 
							  0)) != NULL) {	 
			HText_beginAnchor(me->text, me->inUnderline, ID_A);
			HText_endAnchor(me->text, 0);
		    }
		}
	    }

	     
	    me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
						     NULL,	 
						     href,	 
						     (HTLinkType *) 0);		 
	    FREE(href);
	    me->CurrentANum = HText_beginAnchor(me->text,
						me->inUnderline,
						me->CurrentA);
	    if (me->inBoldH == FALSE)
		HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
	    HTML_put_string(me, alt_string);
	    if (!me->inA) {
		if (me->inBoldH == FALSE)
		    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		HText_endAnchor(me->text, me->CurrentANum);
		me->CurrentANum = 0;
		HTML_put_character(me, ' ');	 
		me->in_word = NO;
	    } else {
		HTML_put_character(me, ' ');	 
		me->in_word = NO;
		me->inBoldA = TRUE;
	    }
	} else if (map_href) {
	    if (me->inA) {
		 
		if (dest_ismap) {
		    HTML_put_character(me, ' ');	 
		    me->in_word = NO;
		    HTML_put_string(me, (temp = MakeNewMapValue(value, "ISMAP")));
		    FREE(temp);
		} else if (dest) {
		    HTML_put_character(me, ' ');	 
		    me->in_word = NO;
		    HTML_put_string(me, "[LINK]");
		}
		if (me->inBoldA == TRUE && me->inBoldH == FALSE) {
		    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		}
		me->inBoldA = FALSE;
		HText_endAnchor(me->text, me->CurrentANum);
		me->CurrentANum = 0;
		if (dest_ismap || dest) {
		    HTML_put_character(me, '-');
		}
	    } else {
		HTML_put_character(me, ' ');
		me->in_word = NO;
	    }
	    me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
						     NULL,	 
						     map_href,	 
						     INTERN_LT);	 
	    if (me->CurrentA && title) {
		if ((dest = HTAnchor_parent(HTAnchor_followLink(me->CurrentA)
		     )) != NULL) {
		    if (!HTAnchor_title(dest))
			HTAnchor_setTitle(dest, title);
		}
	    }
	    me->CurrentANum = HText_beginAnchor(me->text,
						me->inUnderline,
						me->CurrentA);
	    if (me->inBoldA == FALSE && me->inBoldH == FALSE) {
		HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
	    }
	    me->inBoldA = TRUE;
	    HTML_put_string(me, alt_string);
	    if (!me->inA) {
		if (me->inBoldA == TRUE && me->inBoldH == FALSE) {
		    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		}
		me->inBoldA = FALSE;
		HText_endAnchor(me->text, me->CurrentANum);
		me->CurrentANum = 0;
	    }
	} else {
	     
	    HTML_put_character(me, ' ');	 
	    me->in_word = NO;
	    if (id_string) {
		if ((ID_A = HTAnchor_findChildAndLink(me->node_anchor,	 
						      id_string,	 
						      NULL,	 
						      (HTLinkType *) 0)) != NULL) {	 
		    HText_beginAnchor(me->text, me->inUnderline, ID_A);
		    HText_endAnchor(me->text, 0);
		}
	    }
	    HTML_put_string(me, alt_string);
	    HTML_put_character(me, ' ');	 
	    me->in_word = NO;
	}
	FREE(map_href);
	FREE(alt_string);
	FREE(id_string);
	FREE(title);
	FREE(newtitle);
	dest = NULL;
	break;

    case HTML_MAP:
	 
	if (present && present[HTML_MAP_NAME] &&
	    non_empty(value[HTML_MAP_NAME])) {
	    StrAllocCopy(id_string, value[HTML_MAP_NAME]);
	} else if (present && present[HTML_MAP_ID] &&
		   non_empty(value[HTML_MAP_ID])) {
	    StrAllocCopy(id_string, value[HTML_MAP_ID]);
	}
	if (id_string) {
	    TRANSLATE_AND_UNESCAPE_TO_STD(&id_string);
	    if (*id_string == '\0') {
		FREE(id_string);
	    }
	}

	 
	if (!LYMapsOnly)
	    LYHandleID(me, id_string);

	 
	if (id_string) {
	     
	    StrAllocCopy(me->map_address, me->node_anchor->address);
	    if ((cp = StrChr(me->map_address, '#')) != NULL)
		*cp = '\0';
	    StrAllocCat(me->map_address, "#");
	    StrAllocCat(me->map_address, id_string);
	    FREE(id_string);
	    if (present && present[HTML_MAP_TITLE] &&
		non_empty(value[HTML_MAP_TITLE])) {
		StrAllocCopy(title, value[HTML_MAP_TITLE]);
		TRANSLATE_AND_UNESCAPE_ENTITIES(&title, TRUE, FALSE);
		LYTrimHead(title);
		LYTrimTail(title);
		if (*title == '\0') {
		    FREE(title);
		}
	    }
	    LYAddImageMap(me->map_address, title, me->node_anchor);
	    FREE(title);
	}
	break;

    case HTML_AREA:
	if (me->map_address &&
	    present && present[HTML_AREA_HREF] &&
	    non_empty(value[HTML_AREA_HREF])) {
	     
	    StrAllocCopy(href, value[HTML_AREA_HREF]);
	    CHECK_FOR_INTERN(intern_flag, href);
	    (void) LYLegitimizeHREF(me, &href, TRUE, TRUE);

	     
	    Base = (((me->inBASE && *href != '\0') &&
		     !(*href == '#' && LYSeekFragAREAinCur == TRUE))
		    ? me->base_href
		    : me->node_anchor->address);
	    HTParseALL(&href, Base);

	     
	    if (present[HTML_AREA_ALT] &&
		non_empty(value[HTML_AREA_ALT])) {
		StrAllocCopy(alt_string, value[HTML_AREA_ALT]);
	    } else if (present[HTML_AREA_TITLE] &&
		       non_empty(value[HTML_AREA_TITLE])) {
		 
		StrAllocCopy(alt_string, value[HTML_AREA_TITLE]);
	    }
	    if (alt_string != NULL) {
		TRANSLATE_AND_UNESCAPE_ENTITIES(&alt_string,
						me->UsePlainSpace,
						me->HiddenValue);
		 
		LYTrimHead(alt_string);
		LYTrimTail(alt_string);
		if (*alt_string == '\0') {
		    StrAllocCopy(alt_string, href);
		}
	    } else {
		 
		StrAllocCopy(alt_string, href);
	    }

	    LYAddMapElement(me->map_address, href, alt_string,
			    me->node_anchor, intern_flag);
	    FREE(href);
	    FREE(alt_string);
	}
	break;

    case HTML_PARAM:
	 
	break;

    case HTML_BODYTEXT:
	CHECK_ID(HTML_BODYTEXT_ID);
	 
	break;

    case HTML_TEXTFLOW:
	CHECK_ID(HTML_BODYTEXT_ID);
	 
	break;

    case HTML_FIG:
	if (present)
	    LYHandleFIG(me, present, value,
			present[HTML_FIG_ISOBJECT],
			present[HTML_FIG_IMAGEMAP],
			present[HTML_FIG_ID] ? value[HTML_FIG_ID] : NULL,
			present[HTML_FIG_SRC] ? value[HTML_FIG_SRC] : NULL,
			YES, TRUE, &intern_flag);
	else
	    LYHandleFIG(me, NULL, NULL,
			0,
			0,
			NULL,
			NULL, YES, TRUE, &intern_flag);
	break;

    case HTML_OBJECT:
	if (!me->object_started) {
	     
	    if (present) {
		if (present[HTML_OBJECT_DECLARE])
		    me->object_declare = TRUE;
		if (present[HTML_OBJECT_SHAPES])
		    me->object_shapes = TRUE;
		if (present[HTML_OBJECT_ISMAP])
		    me->object_ismap = TRUE;
		if (present[HTML_OBJECT_USEMAP] &&
		    non_empty(value[HTML_OBJECT_USEMAP])) {
		    StrAllocCopy(me->object_usemap, value[HTML_OBJECT_USEMAP]);
		    TRANSLATE_AND_UNESCAPE_TO_STD(&me->object_usemap);
		    if (*me->object_usemap == '\0') {
			FREE(me->object_usemap);
		    }
		}
		if (present[HTML_OBJECT_ID] &&
		    non_empty(value[HTML_OBJECT_ID])) {
		    StrAllocCopy(me->object_id, value[HTML_OBJECT_ID]);
		    TRANSLATE_AND_UNESCAPE_TO_STD(&me->object_id);
		    if (*me->object_id == '\0') {
			FREE(me->object_id);
		    }
		}
		if (present[HTML_OBJECT_TITLE] &&
		    non_empty(value[HTML_OBJECT_TITLE])) {
		    StrAllocCopy(me->object_title, value[HTML_OBJECT_TITLE]);
		    TRANSLATE_AND_UNESCAPE_ENTITIES(&me->object_title, TRUE, FALSE);
		    LYTrimHead(me->object_title);
		    LYTrimTail(me->object_title);
		    if (*me->object_title == '\0') {
			FREE(me->object_title);
		    }
		}
		if (present[HTML_OBJECT_DATA] &&
		    non_empty(value[HTML_OBJECT_DATA])) {
		    StrAllocCopy(me->object_data, value[HTML_OBJECT_DATA]);
		    TRANSLATE_AND_UNESCAPE_TO_STD(&me->object_data);
		    if (*me->object_data == '\0') {
			FREE(me->object_data);
		    }
		}
		if (present[HTML_OBJECT_TYPE] &&
		    non_empty(value[HTML_OBJECT_TYPE])) {
		    StrAllocCopy(me->object_type, value[HTML_OBJECT_TYPE]);
		    TRANSLATE_AND_UNESCAPE_ENTITIES(&me->object_type, TRUE, FALSE);
		    LYTrimHead(me->object_type);
		    LYTrimTail(me->object_type);
		    if (*me->object_type == '\0') {
			FREE(me->object_type);
		    }
		}
		if (present[HTML_OBJECT_CLASSID] &&
		    non_empty(value[HTML_OBJECT_CLASSID])) {
		    StrAllocCopy(me->object_classid,
				 value[HTML_OBJECT_CLASSID]);
		    TRANSLATE_AND_UNESCAPE_ENTITIES(&me->object_classid, TRUE, FALSE);
		    LYTrimHead(me->object_classid);
		    LYTrimTail(me->object_classid);
		    if (*me->object_classid == '\0') {
			FREE(me->object_classid);
		    }
		}
		if (present[HTML_OBJECT_CODEBASE] &&
		    non_empty(value[HTML_OBJECT_CODEBASE])) {
		    StrAllocCopy(me->object_codebase,
				 value[HTML_OBJECT_CODEBASE]);
		    TRANSLATE_AND_UNESCAPE_TO_STD(&me->object_codebase);
		    if (*me->object_codebase == '\0') {
			FREE(me->object_codebase);
		    }
		}
		if (present[HTML_OBJECT_CODETYPE] &&
		    non_empty(value[HTML_OBJECT_CODETYPE])) {
		    StrAllocCopy(me->object_codetype,
				 value[HTML_OBJECT_CODETYPE]);
		    TRANSLATE_AND_UNESCAPE_ENTITIES(&me->object_codetype,
						    TRUE,
						    FALSE);
		    LYTrimHead(me->object_codetype);
		    LYTrimTail(me->object_codetype);
		    if (*me->object_codetype == '\0') {
			FREE(me->object_codetype);
		    }
		}
		if (present[HTML_OBJECT_NAME] &&
		    non_empty(value[HTML_OBJECT_NAME])) {
		    StrAllocCopy(me->object_name, value[HTML_OBJECT_NAME]);
		    TRANSLATE_AND_UNESCAPE_ENTITIES(&me->object_name, TRUE, FALSE);
		    LYTrimHead(me->object_name);
		    LYTrimTail(me->object_name);
		    if (*me->object_name == '\0') {
			FREE(me->object_name);
		    }
		}
	    }
	     
	    if ((!present ||
		 (me->object_declare == FALSE && me->object_name == NULL &&
		  me->object_shapes == FALSE && me->object_usemap == NULL))) {
		if (!LYMapsOnly) {
		    if (!clickable_images || me->object_data == NULL ||
			!(me->object_data != NULL &&
			  me->object_classid == NULL &&
			  me->object_codebase == NULL &&
			  me->object_codetype == NULL))
			FREE(me->object_data);
		    if (me->object_data) {
			HTStartAnchor5(me,
				       (me->object_id
					? value[HTML_OBJECT_ID]
					: NULL),
				       value[HTML_OBJECT_DATA],
				       value[HTML_OBJECT_TYPE],
				       tag_charset);
			if ((me->object_type != NULL) &&
			    !strncasecomp(me->object_type, "image/", 6))
			    HTML_put_string(me, "(IMAGE)");
			else
			    HTML_put_string(me, "(OBJECT)");
			HTML_end_element(me, HTML_A, NULL);
		    } else if (me->object_id)
			LYHandleID(me, me->object_id);
		}
		clear_objectdata(me);
		 
		status = HT_PARSER_OTHER_CONTENT;
		if (me->sp[0].tag_number == HTML_FIG &&
		    me->objects_figged_open > 0) {
		    ElementNumber = (HTMLElement) HTML_OBJECT_M;
		} else {
		    me->objects_mixed_open++;
		    SET_SKIP_STACK(HTML_OBJECT);
		}
	    } else if (me->object_declare == FALSE && me->object_name == NULL &&
		       me->object_shapes == TRUE) {
		LYHandleFIG(me, present, value,
			    1,
			    1 || me->object_ismap,
			    me->object_id,
			    ((me->object_data && !me->object_classid)
			     ? value[HTML_OBJECT_DATA]
			     : NULL),
			    NO, TRUE, &intern_flag);
		clear_objectdata(me);
		status = HT_PARSER_OTHER_CONTENT;
		me->objects_figged_open++;
		ElementNumber = HTML_FIG;

	    } else {
		 
		me->object_started = TRUE;
	    }
	}
	break;

    case HTML_OVERLAY:
	if (clickable_images && me->inFIG &&
	    present && present[HTML_OVERLAY_SRC] &&
	    non_empty(value[HTML_OVERLAY_SRC])) {
	    StrAllocCopy(href, value[HTML_OVERLAY_SRC]);
	    LYLegitimizeHREF(me, &href, TRUE, TRUE);
	    if (*href) {

		if (me->inA) {
		    SET_SKIP_STACK(HTML_A);
		    HTML_end_element(me, HTML_A, include);
		}
		me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
							 NULL,	 
							 href,	 
							 (HTLinkType *) 0);	 
		HTML_put_character(me, ' ');
		HText_appendCharacter(me->text, '+');
		me->CurrentANum = HText_beginAnchor(me->text,
						    me->inUnderline,
						    me->CurrentA);
		if (me->inBoldH == FALSE)
		    HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
		HTML_put_string(me, "[OVERLAY]");
		if (me->inBoldH == FALSE)
		    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		HText_endAnchor(me->text, me->CurrentANum);
		HTML_put_character(me, ' ');
		me->in_word = NO;
	    }
	    FREE(href);
	}
	break;

    case HTML_APPLET:
	me->inAPPLET = TRUE;
	me->inAPPLETwithP = FALSE;
	HTML_put_character(me, ' ');	 
	 
	if (present && present[HTML_APPLET_ID] &&
	    non_empty(value[HTML_APPLET_ID])) {
	    StrAllocCopy(id_string, value[HTML_APPLET_ID]);
	} else if (present && present[HTML_APPLET_NAME] &&
		   non_empty(value[HTML_APPLET_NAME])) {
	    StrAllocCopy(id_string, value[HTML_APPLET_NAME]);
	}
	if (id_string) {
	    TRANSLATE_AND_UNESCAPE_TO_STD(&id_string);
	    LYHandleID(me, id_string);
	    FREE(id_string);
	}
	me->in_word = NO;

	 
	if (present && present[HTML_APPLET_ALT] && value[HTML_APPLET_ALT] &&
	    (!clickable_images ||
	     (clickable_images && *value[HTML_APPLET_ALT] != '\0'))) {
	    StrAllocCopy(alt_string, value[HTML_APPLET_ALT]);
	    TRANSLATE_AND_UNESCAPE_ENTITIES(&alt_string,
					    me->UsePlainSpace, me->HiddenValue);
	     
	    if (clickable_images) {
		LYTrimHead(alt_string);
		LYTrimTail(alt_string);
		if (*alt_string == '\0') {
		    StrAllocCopy(alt_string, "[APPLET]");
		}
	    }

	} else {
	    if (clickable_images)
		StrAllocCopy(alt_string, "[APPLET]");
	    else
		StrAllocCopy(alt_string, "");
	}

	 
	if (clickable_images && present && present[HTML_APPLET_CODE] &&
	    non_empty(value[HTML_APPLET_CODE])) {
	    char *base = NULL;

	    Base = (me->inBASE)
		? me->base_href
		: me->node_anchor->address;
	     
	    if (present[HTML_APPLET_CODEBASE] &&
		non_empty(value[HTML_APPLET_CODEBASE])) {
		StrAllocCopy(base, value[HTML_APPLET_CODEBASE]);
		LYRemoveBlanks(base);
		TRANSLATE_AND_UNESCAPE_TO_STD(&base);
		 
		if (*base == '\0')
		    StrAllocCopy(base, "/");
		LYAddHtmlSep(&base);
		LYLegitimizeHREF(me, &base, TRUE, FALSE);

		HTParseALL(&base, Base);
	    }

	    StrAllocCopy(href, value[HTML_APPLET_CODE]);
	    LYLegitimizeHREF(me, &href, TRUE, FALSE);
	    HTParseALL(&href, (base ? base : Base));
	    FREE(base);

	    if (*href) {
		if (me->inA) {
		    if (me->inBoldA == TRUE && me->inBoldH == FALSE)
			HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		    HText_endAnchor(me->text, me->CurrentANum);
		    HTML_put_character(me, '-');
		}
		me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
							 NULL,	 
							 href,	 
							 (HTLinkType *) 0);	 
		me->CurrentANum = HText_beginAnchor(me->text,
						    me->inUnderline,
						    me->CurrentA);
		if (me->inBoldH == FALSE)
		    HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
		HTML_put_string(me, alt_string);
		if (me->inA == FALSE) {
		    if (me->inBoldH == FALSE)
			HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		    HText_endAnchor(me->text, me->CurrentANum);
		    me->CurrentANum = 0;
		}
		HTML_put_character(me, ' ');	 
		me->in_word = NO;
	    }
	    FREE(href);
	} else if (*alt_string) {
	     
	    HTML_put_string(me, alt_string);
	    HTML_put_character(me, ' ');	 
	    me->in_word = NO;
	}
	FREE(alt_string);
	FREE(id_string);
	break;

    case HTML_BGSOUND:
	 
	if (clickable_images && present && present[HTML_BGSOUND_SRC] &&
	    non_empty(value[HTML_BGSOUND_SRC])) {
	    StrAllocCopy(href, value[HTML_BGSOUND_SRC]);
	    LYLegitimizeHREF(me, &href, TRUE, TRUE);
	    if (*href == '\0') {
		FREE(href);
		break;
	    }

	    if (me->inA) {
		if (me->inBoldA == TRUE && me->inBoldH == FALSE)
		    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		HText_endAnchor(me->text, me->CurrentANum);
		HTML_put_character(me, '-');
	    } else {
		HTML_put_character(me, ' ');	 
		me->in_word = NO;
	    }
	    me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
						     NULL,	 
						     href,	 
						     (HTLinkType *) 0);		 
	    me->CurrentANum = HText_beginAnchor(me->text,
						me->inUnderline,
						me->CurrentA);
	    if (me->inBoldH == FALSE)
		HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
	    HTML_put_string(me, "[BGSOUND]");
	    if (me->inA == FALSE) {
		if (me->inBoldH == FALSE)
		    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		HText_endAnchor(me->text, me->CurrentANum);
		me->CurrentANum = 0;
	    }
	    HTML_put_character(me, ' ');	 
	    me->in_word = NO;
	    FREE(href);
	}
	break;

    case HTML_EMBED:
	if (pseudo_inline_alts || clickable_images)
	    HTML_put_character(me, ' ');	 
	 
	if (present && present[HTML_EMBED_ID] &&
	    non_empty(value[HTML_EMBED_ID])) {
	    StrAllocCopy(id_string, value[HTML_EMBED_ID]);
	} else if (present && present[HTML_EMBED_NAME] &&
		   non_empty(value[HTML_EMBED_NAME])) {
	    StrAllocCopy(id_string, value[HTML_EMBED_NAME]);
	}
	if (id_string) {
	    TRANSLATE_AND_UNESCAPE_TO_STD(&id_string);
	    LYHandleID(me, id_string);
	    FREE(id_string);
	}
	if (pseudo_inline_alts || clickable_images)
	    me->in_word = NO;

	 
	if (present && present[HTML_EMBED_ALT] && value[HTML_EMBED_ALT] &&
	    (!clickable_images ||
	     (clickable_images && *value[HTML_EMBED_ALT] != '\0'))) {
	    StrAllocCopy(alt_string, value[HTML_EMBED_ALT]);
	    TRANSLATE_AND_UNESCAPE_ENTITIES(&alt_string,
					    me->UsePlainSpace, me->HiddenValue);
	     
	    if (clickable_images) {
		LYTrimHead(alt_string);
		LYTrimTail(alt_string);
		if (*alt_string == '\0') {
		    StrAllocCopy(alt_string, "[EMBED]");
		}
	    }
	} else {
	    if (pseudo_inline_alts || clickable_images)
		StrAllocCopy(alt_string, "[EMBED]");
	    else
		StrAllocCopy(alt_string, "");
	}

	 
	if (clickable_images && present && present[HTML_EMBED_SRC] &&
	    non_empty(value[HTML_EMBED_SRC])) {
	    StrAllocCopy(href, value[HTML_EMBED_SRC]);
	    LYLegitimizeHREF(me, &href, TRUE, TRUE);
	    if (*href) {
		if (me->inA) {
		    if (me->inBoldA == TRUE && me->inBoldH == FALSE)
			HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		    HText_endAnchor(me->text, me->CurrentANum);
		    HTML_put_character(me, '-');
		}
		me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
							 NULL,	 
							 href,	 
							 (HTLinkType *) 0);	 
		me->CurrentANum = HText_beginAnchor(me->text,
						    me->inUnderline,
						    me->CurrentA);
		if (me->inBoldH == FALSE)
		    HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
		HTML_put_string(me, alt_string);
		if (me->inBoldH == FALSE)
		    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		if (me->inA == FALSE) {
		    if (me->inBoldH == FALSE)
			HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		    HText_endAnchor(me->text, me->CurrentANum);
		    me->CurrentANum = 0;
		}
		HTML_put_character(me, ' ');
		me->in_word = NO;
	    }
	    FREE(href);
	} else if (*alt_string) {
	     
	    HTML_put_string(me, alt_string);
	    HTML_put_character(me, ' ');	 
	    me->in_word = NO;
	}
	FREE(alt_string);
	FREE(id_string);
	break;

    case HTML_CREDIT:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	me->inCREDIT = TRUE;
	CHECK_ID(HTML_GEN_ID);
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	HTML_put_string(me, "CREDIT:");
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	HTML_put_character(me, ' ');
	CAN_JUSTIFY_START;

	if (me->inFIG)
	     
	    me->inFIGwithP = TRUE;

	if (me->inAPPLET)
	     
	    me->inAPPLETwithP = TRUE;

	me->inLABEL = TRUE;
	me->in_word = NO;
	me->inP = FALSE;
	break;

    case HTML_CAPTION:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	me->inCAPTION = TRUE;
	CHECK_ID(HTML_CAPTION_ID);
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	HTML_put_string(me, "CAPTION:");
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	HTML_put_character(me, ' ');
	CAN_JUSTIFY_START;

	if (me->inFIG)
	     
	    me->inFIGwithP = TRUE;

	if (me->inAPPLET)
	     
	    me->inAPPLETwithP = TRUE;

	me->inLABEL = TRUE;
	me->in_word = NO;
	me->inP = FALSE;
	break;

    case HTML_FORM:
	{
	    char *action = NULL;
	    char *method = NULL;
	    char *enctype = NULL;
	    const char *accept_cs = NULL;

	    HTChildAnchor *source;
	    HTAnchor *link_dest;

	     
	    if (me->inFORM) {
		CTRACE((tfp, "HTML: Missing FORM end tag.  Faking it!\n"));
		SET_SKIP_STACK(HTML_FORM);
		HTML_end_element(me, HTML_FORM, include);
	    }

	     
	    me->inFORM = TRUE;
	    EMIT_IFDEF_USE_JUSTIFY_ELTS(form_in_htext = TRUE);

	    if (present && present[HTML_FORM_ACCEPT_CHARSET]) {
		accept_cs = (value[HTML_FORM_ACCEPT_CHARSET]
			     ? value[HTML_FORM_ACCEPT_CHARSET]
			     : "UNKNOWN");
	    }

	    Base = (me->inBASE)
		? me->base_href
		: me->node_anchor->address;

	    if (present && present[HTML_FORM_ACTION] &&
		value[HTML_FORM_ACTION]) {

		StrAllocCopy(action, value[HTML_FORM_ACTION]);
		LYLegitimizeHREF(me, &action, TRUE, TRUE);

		 
		HTParseALL(&action, Base);

	    } else {
		StrAllocCopy(action, Base);
	    }

	    source = HTAnchor_findChildAndLink(me->node_anchor,
					       NULL,
					       action,
					       (HTLinkType *) 0);
	    if ((link_dest = HTAnchor_followLink(source)) != NULL) {
		 
		char *cp_freeme = HTAnchor_address(link_dest);

		if (cp_freeme != NULL) {
		    StrAllocCopy(action, cp_freeme);
		    FREE(cp_freeme);
		} else {
		    StrAllocCopy(action, "");
		}
	    }

	    if (present && present[HTML_FORM_METHOD])
		StrAllocCopy(method, (value[HTML_FORM_METHOD]
				      ? value[HTML_FORM_METHOD]
				      : "GET"));

	    if (present && present[HTML_FORM_ENCTYPE] &&
		non_empty(value[HTML_FORM_ENCTYPE])) {
		StrAllocCopy(enctype, value[HTML_FORM_ENCTYPE]);
		LYLowerCase(enctype);
	    }

	    if (present) {
		 
		if (present[HTML_FORM_TITLE] &&
		    non_empty(value[HTML_FORM_TITLE])) {
		    StrAllocCopy(title, value[HTML_FORM_TITLE]);
		} else if (present[HTML_FORM_SUBJECT] &&
			   non_empty(value[HTML_FORM_SUBJECT])) {
		    StrAllocCopy(title, value[HTML_FORM_SUBJECT]);
		}
		if (non_empty(title)) {
		    TRANSLATE_AND_UNESCAPE_ENTITIES(&title, TRUE, FALSE);
		    LYTrimHead(title);
		    LYTrimTail(title);
		    if (*title == '\0') {
			FREE(title);
		    }
		}
	    }

	    HText_beginForm(action, method, enctype, title, accept_cs);

	    FREE(action);
	    FREE(method);
	    FREE(enctype);
	    FREE(title);
	}
	CHECK_ID(HTML_FORM_ID);
	break;

    case HTML_FIELDSET:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	CHECK_ID(HTML_GEN_ID);
	break;

    case HTML_LEGEND:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	CHECK_ID(HTML_CAPTION_ID);
	break;

    case HTML_LABEL:
	CHECK_ID(HTML_LABEL_ID);
	break;

    case HTML_KEYGEN:
	CHECK_ID(HTML_KEYGEN_ID);
	break;

    case HTML_BUTTON:
	{
	    InputFieldData I;
	    int chars;

	     
	    memset(&I, 0, sizeof(I));
	    I.name_cs = ATTR_CS_IN;
	    I.value_cs = ATTR_CS_IN;

	    UPDATE_STYLE;
	    if (present &&
		present[HTML_BUTTON_TYPE] &&
		value[HTML_BUTTON_TYPE]) {
		if (!strcasecomp(value[HTML_BUTTON_TYPE], "submit") ||
		    !strcasecomp(value[HTML_BUTTON_TYPE], "reset")) {
		     
		    I.type = value[HTML_BUTTON_TYPE];
		} else {
		     
		    I.type = value[HTML_BUTTON_TYPE];
		    CTRACE((tfp, "found button for a script\n"));
		}
	    } else {
		 
		I.type = "submit";
	    }

	     
	    if (me->sp[0].tag_number != HTML_PRE && !me->inPRE &&
		me->sp->style->freeFormat) {
		HTML_put_character(me, ' ');
		me->in_word = NO;
	    } else if (HText_LastLineSize(me->text, FALSE) > (LYcolLimit - 6)) {
		HTML_put_character(me, '\n');
		me->in_word = NO;
	    }
	    HTML_put_character(me, '(');

	    if (!(present && present[HTML_BUTTON_NAME] &&
		  value[HTML_BUTTON_NAME])) {
		I.name = "";
	    } else if (StrChr(value[HTML_BUTTON_NAME], '&') == NULL) {
		I.name = value[HTML_BUTTON_NAME];
	    } else {
		StrAllocCopy(I_name, value[HTML_BUTTON_NAME]);
		UNESCAPE_FIELDNAME_TO_STD(&I_name);
		I.name = I_name;
	    }

	    if (present && present[HTML_BUTTON_VALUE] &&
		non_empty(value[HTML_BUTTON_VALUE])) {
		 
		StrAllocCopy(I_value, value[HTML_BUTTON_VALUE]);
		me->UsePlainSpace = TRUE;
		TRANSLATE_AND_UNESCAPE_ENTITIES(&I_value, TRUE, me->HiddenValue);
		me->UsePlainSpace = FALSE;
		I.value = I_value;
		 
		LYReduceBlanks(I.value);
	    } else if (!strcasecomp(I.type, "button")) {
		if (non_empty(I.name)) {
		    StrAllocCopy(I.value, I.name);
		} else {
		    StrAllocCopy(I.value, "BUTTON");
		}
	    } else if (I.value == 0) {
		StrAllocCopy(I.value, "BUTTON");
	    }

	    if (present && present[HTML_BUTTON_READONLY])
		I.readonly = YES;

	    if (present && present[HTML_BUTTON_DISABLED])
		I.disabled = YES;

	    if (present && present[HTML_BUTTON_CLASS] &&	 
		non_empty(value[HTML_BUTTON_CLASS]))
		I.iclass = value[HTML_BUTTON_CLASS];

	    if (present && present[HTML_BUTTON_ID] &&
		non_empty(value[HTML_BUTTON_ID])) {
		I.id = value[HTML_BUTTON_ID];
		CHECK_ID(HTML_BUTTON_ID);
	    }

	    if (present && present[HTML_BUTTON_LANG] &&		 
		non_empty(value[HTML_BUTTON_LANG]))
		I.lang = value[HTML_BUTTON_LANG];

	    chars = HText_beginInput(me->text, me->inUnderline, &I);
	     
	    if (me->sp[0].tag_number == HTML_PRE ||
		!me->sp->style->freeFormat) {
		 
		while (I.value[i])
		    HTML_put_character(me, I.value[i++]);
	    } else {
		 
		for (i = 0; I.value[i]; i++) {
		    HTML_put_character(me,
				       (char) ((I.value[i] == ' ')
					       ? HT_NON_BREAK_SPACE
					       : I.value[i]));
		}
		while (i++ < chars) {
		    HTML_put_character(me, HT_NON_BREAK_SPACE);
		}
	    }
	    HTML_put_character(me, ')');
	    if (me->sp[0].tag_number != HTML_PRE &&
		me->sp->style->freeFormat) {
		HTML_put_character(me, ' ');
		me->in_word = NO;
	    }
	    FREE(I_value);
	    FREE(I_name);
	}
	break;

    case HTML_INPUT:
	{
	    InputFieldData I;
	    int chars;
	    BOOL UseALTasVALUE = FALSE;
	    BOOL HaveSRClink = FALSE;
	    char *ImageSrc = NULL;
	    BOOL IsSubmitOrReset = FALSE;
	    HTkcode kcode = NOKANJI;
	    HTkcode specified_kcode = NOKANJI;

	     
	    memset(&I, 0, sizeof(I));
	    I.name_cs = ATTR_CS_IN;
	    I.value_cs = ATTR_CS_IN;

	    UPDATE_STYLE;

	     
	    if (me->sp[0].tag_number != HTML_PRE && !me->inPRE &&
		me->sp->style->freeFormat) {
		HTML_put_character(me, ' ');
		me->in_word = NO;
	    } else if (HText_LastLineSize(me->text, FALSE) > (LYcolLimit - 6)) {
		HTML_put_character(me, '\n');
		me->in_word = NO;
	    }

	     
	    if (present && present[HTML_INPUT_TYPE] &&
		non_empty(value[HTML_INPUT_TYPE])) {
		const char *not_impl = NULL;
		char *usingval = NULL;

		I.type = value[HTML_INPUT_TYPE];

		if (!strcasecomp(I.type, "range")) {
		    if (present[HTML_INPUT_MIN] &&
			non_empty(value[HTML_INPUT_MIN]))
			I.min = value[HTML_INPUT_MIN];
		    if (present[HTML_INPUT_MAX] &&
			non_empty(value[HTML_INPUT_MAX]))
			I.max = value[HTML_INPUT_MAX];
		     
#ifdef NOTDEFINED
		    not_impl = "[RANGE Input]";
		    if (me->inFORM)
			HText_DisableCurrentForm();
#endif  
		    CTRACE((tfp, "HTML: Ignoring TYPE=\"range\"\n"));
		    break;

		} else if (!strcasecomp(I.type, "file")) {
		    if (present[HTML_INPUT_ACCEPT] &&
			non_empty(value[HTML_INPUT_ACCEPT]))
			I.accept = value[HTML_INPUT_ACCEPT];
#ifndef USE_FILE_UPLOAD
		    not_impl = "[FILE Input]";
		    CTRACE((tfp, "Attempting to fake as: %s\n", I.type));
#ifdef NOTDEFINED
		    if (me->inFORM)
			HText_DisableCurrentForm();
#endif  
		    CTRACE((tfp, "HTML: Ignoring TYPE=\"file\"\n"));
#endif  

		} else if (!strcasecomp(I.type, "button")) {
		     
		    not_impl = "[BUTTON Input]";
		}
		if (not_impl != NULL) {
		    if (me->inUnderline == FALSE) {
			HText_appendCharacter(me->text,
					      LY_UNDERLINE_START_CHAR);
		    }
		    HTML_put_string(me, not_impl);
		    if (usingval != NULL) {
			HTML_put_string(me, usingval);
			FREE(usingval);
		    } else {
			HTML_put_string(me, " (not implemented)");
		    }
		    if (me->inUnderline == FALSE) {
			HText_appendCharacter(me->text,
					      LY_UNDERLINE_END_CHAR);
		    }
		}
	    }

	    CTRACE((tfp, "Ok, we're trying type=[%s]\n", NONNULL(I.type)));

	     
	    if (me->inTEXTAREA) {
		if (LYBadHTML(me)) {
		    LYShowBadHTML("Bad HTML: Missing TEXTAREA end tag.\n");
		}
	    }

	     
	    if (me->inSELECT) {
		CTRACE((tfp, "HTML: Missing SELECT end tag, faking it...\n"));
		if (me->sp->tag_number != HTML_SELECT) {
		    SET_SKIP_STACK(HTML_SELECT);
		}
		HTML_end_element(me, HTML_SELECT, include);
	    }

	     
	    if (!(present && present[HTML_INPUT_NAME] &&
		  non_empty(value[HTML_INPUT_NAME]))) {
		I.name = "";
	    } else if (StrChr(value[HTML_INPUT_NAME], '&') == NULL) {
		I.name = value[HTML_INPUT_NAME];
	    } else {
		StrAllocCopy(I_name, value[HTML_INPUT_NAME]);
		UNESCAPE_FIELDNAME_TO_STD(&I_name);
		I.name = I_name;
	    }

	    if ((present && present[HTML_INPUT_ALT] &&
		 non_empty(value[HTML_INPUT_ALT]) &&
		 I.type && !strcasecomp(I.type, "image")) &&
		!(present && present[HTML_INPUT_VALUE] &&
		  non_empty(value[HTML_INPUT_VALUE]))) {
		 
		UseALTasVALUE = TRUE;
	    }
	    if (verbose_img && !clickable_images &&
		present && present[HTML_INPUT_SRC] &&
		non_empty(value[HTML_INPUT_SRC]) &&
		I.type && !strcasecomp(I.type, "image")) {
		ImageSrc = MakeNewImageValue(value);
	    } else if (clickable_images == TRUE &&
		       present && present[HTML_INPUT_SRC] &&
		       non_empty(value[HTML_INPUT_SRC]) &&
		       I.type && !strcasecomp(I.type, "image")) {
		StrAllocCopy(href, value[HTML_INPUT_SRC]);
		 
		LYLegitimizeHREF(me, &href, TRUE, TRUE);
		if (*href) {

		    if (me->inA) {
			SET_SKIP_STACK(HTML_A);
			HTML_end_element(me, HTML_A, include);
		    }
		    me->CurrentA = HTAnchor_findChildAndLink(me->node_anchor,	 
							     NULL,	 
							     href,	 
							     (HTLinkType *) 0);		 
		    HText_beginAnchor(me->text, me->inUnderline, me->CurrentA);
		    if (me->inBoldH == FALSE)
			HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
		    HTML_put_string(me, VERBOSE_IMG(value,
						    HTML_INPUT_SRC,
						    "[IMAGE]"));
		    FREE(newtitle);
		    if (me->inBoldH == FALSE)
			HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
		    HText_endAnchor(me->text, 0);
		    HTML_put_character(me, '-');
		    HaveSRClink = TRUE;
		}
		FREE(href);
	    }
	    CTRACE((tfp, "2.Ok, we're trying type=[%s] (present=%p)\n",
		    NONNULL(I.type),
		    (const void *) present));
	     
	    if ((UseALTasVALUE == TRUE) ||
		(present && present[HTML_INPUT_VALUE] &&
		 value[HTML_INPUT_VALUE] &&
		 (*value[HTML_INPUT_VALUE] ||
		  (I.type && (!strcasecomp(I.type, "checkbox") ||
			      !strcasecomp(I.type, "radio")))))) {

		 
		int CurrentCharSet = current_char_set;
		BOOL CurrentEightBitRaw = HTPassEightBitRaw;
		BOOLEAN CurrentUseDefaultRawMode = LYUseDefaultRawMode;
		HTCJKlang CurrentHTCJK = HTCJK;

		if (I.type && !strcasecomp(I.type, "hidden")) {
		    me->HiddenValue = TRUE;
		    current_char_set = LATIN1;	 
		    LYUseDefaultRawMode = TRUE;
		    HTMLSetCharacterHandling(current_char_set);
		}

		CTRACE((tfp, "3.Ok, we're trying type=[%s]\n", NONNULL(I.type)));
		if (!I.type)
		    me->UsePlainSpace = TRUE;
		else if (!strcasecomp(I.type, "text") ||
#ifdef USE_FILE_UPLOAD
			 !strcasecomp(I.type, "file") ||
#endif
			 !strcasecomp(I.type, "submit") ||
			 !strcasecomp(I.type, "image") ||
			 !strcasecomp(I.type, "reset")) {
		    CTRACE((tfp, "normal field type: %s\n", NONNULL(I.type)));
		    me->UsePlainSpace = TRUE;
		}

		StrAllocCopy(I_value,
			     ((UseALTasVALUE == TRUE)
			      ? value[HTML_INPUT_ALT]
			      : value[HTML_INPUT_VALUE]));
		if (me->UsePlainSpace && !me->HiddenValue) {
		    I.value_cs = current_char_set;
		}
		CTRACE((tfp, "4.Ok, we're trying type=[%s]\n", NONNULL(I.type)));
		TRANSLATE_AND_UNESCAPE_ENTITIES6(&I_value,
						 ATTR_CS_IN,
						 I.value_cs,
						 (BOOL) (me->UsePlainSpace &&
							 !me->HiddenValue),
						 me->UsePlainSpace,
						 me->HiddenValue);
		I.value = I_value;
		if (me->UsePlainSpace == TRUE) {
		     
		    LYReduceBlanks(I.value);
		}
		me->UsePlainSpace = FALSE;

		if (I.type && !strcasecomp(I.type, "hidden")) {
		    me->HiddenValue = FALSE;
		    current_char_set = CurrentCharSet;
		    LYUseDefaultRawMode = CurrentUseDefaultRawMode;
		    HTMLSetCharacterHandling(current_char_set);
		    HTPassEightBitRaw = CurrentEightBitRaw;
		    HTCJK = CurrentHTCJK;
		}
	    } else if (HaveSRClink == TRUE) {
		 
		StrAllocCopy(I_value, "Submit");
		I.value = I_value;
	    } else if (ImageSrc) {
		 
		I.value = ImageSrc;
	    }
	    if (present && present[HTML_INPUT_READONLY])
		I.readonly = YES;
	    if (present && present[HTML_INPUT_CHECKED])
		I.checked = YES;
	    if (present && present[HTML_INPUT_SIZE] &&
		non_empty(value[HTML_INPUT_SIZE]))
		I.size = atoi(value[HTML_INPUT_SIZE]);
	    LimitValue(I.size, MAX_LINE);
	    if (present && present[HTML_INPUT_MAXLENGTH] &&
		non_empty(value[HTML_INPUT_MAXLENGTH]))
		I.maxlength = value[HTML_INPUT_MAXLENGTH];
	    if (present && present[HTML_INPUT_DISABLED])
		I.disabled = YES;

	    if (present && present[HTML_INPUT_ACCEPT_CHARSET]) {	 
		I.accept_cs = (value[HTML_INPUT_ACCEPT_CHARSET]
			       ? value[HTML_INPUT_ACCEPT_CHARSET]
			       : "UNKNOWN");
	    }
	    if (present && present[HTML_INPUT_ALIGN] &&		 
		non_empty(value[HTML_INPUT_ALIGN]))
		I.align = value[HTML_INPUT_ALIGN];
	    if (present && present[HTML_INPUT_CLASS] &&		 
		non_empty(value[HTML_INPUT_CLASS]))
		I.iclass = value[HTML_INPUT_CLASS];
	    if (present && present[HTML_INPUT_ERROR] &&		 
		non_empty(value[HTML_INPUT_ERROR]))
		I.error = value[HTML_INPUT_ERROR];
	    if (present && present[HTML_INPUT_HEIGHT] &&	 
		non_empty(value[HTML_INPUT_HEIGHT]))
		I.height = value[HTML_INPUT_HEIGHT];
	    if (present && present[HTML_INPUT_WIDTH] &&		 
		non_empty(value[HTML_INPUT_WIDTH]))
		I.width = value[HTML_INPUT_WIDTH];
	    if (present && present[HTML_INPUT_ID] &&
		non_empty(value[HTML_INPUT_ID])) {
		I.id = value[HTML_INPUT_ID];
		CHECK_ID(HTML_INPUT_ID);
	    }
	    if (present && present[HTML_INPUT_LANG] &&	 
		non_empty(value[HTML_INPUT_LANG]))
		I.lang = value[HTML_INPUT_LANG];
	    if (present && present[HTML_INPUT_MD] &&	 
		non_empty(value[HTML_INPUT_MD]))
		I.md = value[HTML_INPUT_MD];

	    chars = HText_beginInput(me->text, me->inUnderline, &I);
	    CTRACE((tfp,
		    "I.%s have %d chars, or something\n",
		    NONNULL(I.type),
		    chars));
	     
	    if (I.type &&
		(!strcasecomp(I.type, "submit") ||
		 !strcasecomp(I.type, "reset") ||
		 !strcasecomp(I.type, "image")))
		IsSubmitOrReset = TRUE;

	    if (I.type && chars == 3 &&
		!strcasecomp(I.type, "radio")) {
		 
		HTML_put_string(me, "(_)");
		HText_endInput(me->text);
		chars = 0;
		me->in_word = YES;
		if (me->sp[0].tag_number != HTML_PRE &&
		    me->sp->style->freeFormat) {
		    HTML_put_character(me, ' ');
		    me->in_word = NO;
		}
	    } else if (I.type && chars == 3 &&
		       !strcasecomp(I.type, "checkbox")) {
		 
		HTML_put_string(me, "[_]");
		HText_endInput(me->text);
		chars = 0;
		me->in_word = YES;
		if (me->sp[0].tag_number != HTML_PRE &&
		    me->sp->style->freeFormat) {
		    HTML_put_character(me, ' ');
		    me->in_word = NO;
		}
	    } else if ((me->sp[0].tag_number == HTML_PRE ||
			!me->sp->style->freeFormat)
		       && chars > 6 &&
		       IsSubmitOrReset == FALSE) {
		 
		for (i = 0; i < 6; i++) {
		    HTML_put_character(me, '_');
		    chars--;
		}
	    }
	    CTRACE((tfp, "I.%s, %d\n", NONNULL(I.type), IsSubmitOrReset));
	    if (IsSubmitOrReset == FALSE) {
		 
		if (chars > 0) {
		    for (; chars > 0; chars--)
			HTML_put_character(me, '_');
		    HText_endInput(me->text);
		}
	    } else {
		if (HTCJK == JAPANESE) {
		    kcode = HText_getKcode(me->text);
		    HText_updateKcode(me->text, kanji_code);
		    specified_kcode = HText_getSpecifiedKcode(me->text);
		    HText_updateSpecifiedKcode(me->text, kanji_code);
		}
		if (me->sp[0].tag_number == HTML_PRE ||
		    !me->sp->style->freeFormat) {
		     
		    while (I.value[i])
			HTML_put_character(me, I.value[i++]);
		} else {
		     
		    for (i = 0; I.value[i]; i++)
			HTML_put_character(me,
					   (char) (I.value[i] == ' '
						   ? HT_NON_BREAK_SPACE
						   : I.value[i]));
		    while (i++ < chars)
			HTML_put_character(me, HT_NON_BREAK_SPACE);
		}
		if (HTCJK == JAPANESE) {
		    HText_updateKcode(me->text, kcode);
		    HText_updateSpecifiedKcode(me->text, specified_kcode);
		}
	    }
	    if (chars != 0) {
		HText_endInput(me->text);
	    }
	    FREE(ImageSrc);
	    FREE(I_value);
	    FREE(I_name);
	}
	break;

    case HTML_TEXTAREA:
	 
	me->inTEXTAREA = TRUE;

	 
	HTChunkClear(&me->textarea);
	if (present && present[HTML_TEXTAREA_NAME] &&
	    value[HTML_TEXTAREA_NAME]) {
	    StrAllocCopy(me->textarea_name, value[HTML_TEXTAREA_NAME]);
	    me->textarea_name_cs = ATTR_CS_IN;
	    if (StrChr(value[HTML_TEXTAREA_NAME], '&') != NULL) {
		UNESCAPE_FIELDNAME_TO_STD(&me->textarea_name);
	    }
	} else {
	    StrAllocCopy(me->textarea_name, "");
	}

	if (present && present[HTML_TEXTAREA_ACCEPT_CHARSET]) {
	    if (value[HTML_TEXTAREA_ACCEPT_CHARSET]) {
		StrAllocCopy(me->textarea_accept_cs, value[HTML_TEXTAREA_ACCEPT_CHARSET]);
		TRANSLATE_AND_UNESCAPE_TO_STD(&me->textarea_accept_cs);
	    } else {
		StrAllocCopy(me->textarea_accept_cs, "UNKNOWN");
	    }
	} else {
	    FREE(me->textarea_accept_cs);
	}

	if (present && present[HTML_TEXTAREA_COLS] &&
	    value[HTML_TEXTAREA_COLS] &&
	    isdigit(UCH(*value[HTML_TEXTAREA_COLS]))) {
	    me->textarea_cols = atoi(value[HTML_TEXTAREA_COLS]);
	} else {
	    int width;

	    width = LYcolLimit -
		me->new_style->leftIndent - me->new_style->rightIndent;
	    if (dump_output_immediately)	 
		width = HTMIN(width, DFT_TEXTAREA_COLS);
	    if (width > 1 && (width - 1) * 6 < MAX_LINE - 3 -
		me->new_style->leftIndent - me->new_style->rightIndent)
		me->textarea_cols = width;
	    else
		me->textarea_cols = DFT_TEXTAREA_COLS;
	}
	LimitValue(me->textarea_cols, MAX_TEXTAREA_COLS);

	if (present && present[HTML_TEXTAREA_ROWS] &&
	    value[HTML_TEXTAREA_ROWS] &&
	    isdigit(UCH(*value[HTML_TEXTAREA_ROWS]))) {
	    me->textarea_rows = atoi(value[HTML_TEXTAREA_ROWS]);
	} else {
	    me->textarea_rows = DFT_TEXTAREA_ROWS;
	}
	LimitValue(me->textarea_rows, MAX_TEXTAREA_ROWS);

	 
	me->textarea_readonly = NO;
	if (present && present[HTML_TEXTAREA_READONLY])
	    me->textarea_readonly = YES;

	me->textarea_disabled = NO;
	if (present && present[HTML_TEXTAREA_DISABLED])
	    me->textarea_disabled = YES;

	if (present && present[HTML_TEXTAREA_ID]
	    && non_empty(value[HTML_TEXTAREA_ID])) {
	    StrAllocCopy(id_string, value[HTML_TEXTAREA_ID]);
	    TRANSLATE_AND_UNESCAPE_TO_STD(&id_string);
	    if ((*id_string != '\0') &&
		(ID_A = HTAnchor_findChildAndLink(me->node_anchor,	 
						  id_string,	 
						  NULL,		 
						  (HTLinkType *) 0))) {		 
		HText_beginAnchor(me->text, me->inUnderline, ID_A);
		HText_endAnchor(me->text, 0);
		StrAllocCopy(me->textarea_id, id_string);
	    } else {
		FREE(me->textarea_id);
	    }
	    FREE(id_string);
	} else {
	    FREE(me->textarea_id);
	}
	break;

    case HTML_SELECT:
	 
	if (me->inSELECT) {
	    if (LYBadHTML(me)) {
		LYShowBadHTML("Bad HTML: SELECT start tag in SELECT element.  Faking SELECT end tag. *****\n");
	    }
	    if (me->sp->tag_number != HTML_SELECT) {
		SET_SKIP_STACK(HTML_SELECT);
	    }
	    HTML_end_element(me, HTML_SELECT, include);
	}

	 
	LYHandleSELECT(me,
		       present, (STRING2PTR) value,
		       include,
		       TRUE);
	break;

    case HTML_OPTION:
	{
	     
	    InputFieldData I;

	     
	    if (!me->inSELECT) {
		if (LYBadHTML(me)) {
		    LYShowBadHTML("Bad HTML: OPTION tag not within SELECT tag\n");
		}

		 
		break;
	    }

	    if (!me->first_option) {
		 
		HTChunkTerminate(&me->option);

		 
		HText_setLastOptionValue(me->text,
					 me->option.data,
					 me->LastOptionValue,
					 MIDDLE_ORDER,
					 me->LastOptionChecked,
					 me->UCLYhndl,
					 ATTR_CS_IN);
	    }

	     
	    if (me->first_option ||
		HTCurSelectGroupType == F_CHECKBOX_TYPE ||
		LYSelectPopups == FALSE) {
		if (HTCurSelectGroupType == F_CHECKBOX_TYPE ||
		    LYSelectPopups == FALSE) {
		     
		    LYEnsureSingleSpace(me);
		} else {
		     
		    HText_appendCharacter(me->text, '[');
		    me->in_word = YES;
		}

		 
		memset(&I, 0, sizeof(I));
		I.name_cs = -1;
		I.value_cs = current_char_set;

		I.type = "OPTION";

		if ((present && present[HTML_OPTION_SELECTED]) ||
		    (me->first_option && LYSelectPopups == FALSE &&
		     HTCurSelectGroupType == F_RADIO_TYPE))
		    I.checked = YES;

		if (present && present[HTML_OPTION_VALUE] &&
		    value[HTML_OPTION_VALUE]) {
		     
		    StrAllocCopy(I_value, value[HTML_OPTION_VALUE]);
		    me->HiddenValue = TRUE;
		    TRANSLATE_AND_UNESCAPE_ENTITIES6(&I_value,
						     ATTR_CS_IN,
						     ATTR_CS_IN,
						     NO,
						     me->UsePlainSpace, me->HiddenValue);
		    I.value_cs = ATTR_CS_IN;
		    me->HiddenValue = FALSE;

		    I.value = I_value;
		}

		if (me->select_disabled ||
		    (0 && present && present[HTML_OPTION_DISABLED])) {
		     
		    I.disabled = YES;
		}

		if (present && present[HTML_OPTION_ID]
		    && non_empty(value[HTML_OPTION_ID])) {
		    if ((ID_A = HTAnchor_findChildAndLink(me->node_anchor,	 
							  value[HTML_OPTION_ID],	 
							  NULL,		 
							  0)) != NULL) {	 
			HText_beginAnchor(me->text, me->inUnderline, ID_A);
			HText_endAnchor(me->text, 0);
			I.id = value[HTML_OPTION_ID];
		    }
		}

		HText_beginInput(me->text, me->inUnderline, &I);

		if (HTCurSelectGroupType == F_CHECKBOX_TYPE) {
		     
		    HText_appendCharacter(me->text, '[');
		    HText_appendCharacter(me->text, '_');
		    HText_appendCharacter(me->text, ']');
		    HText_appendCharacter(me->text, ' ');
		    HText_setLastChar(me->text, ' ');	 
		    me->in_word = NO;
		} else if (LYSelectPopups == FALSE) {
		     
		    HText_appendCharacter(me->text, '(');
		    HText_appendCharacter(me->text, '_');
		    HText_appendCharacter(me->text, ')');
		    HText_appendCharacter(me->text, ' ');
		    HText_setLastChar(me->text, ' ');	 
		    me->in_word = NO;
		}
	    }

	     
	    HTChunkClear(&me->option);
	    if ((present && present[HTML_OPTION_SELECTED]) ||
		(me->first_option && LYSelectPopups == FALSE &&
		 HTCurSelectGroupType == F_RADIO_TYPE))
		me->LastOptionChecked = TRUE;
	    else
		me->LastOptionChecked = FALSE;
	    me->first_option = FALSE;

	    if (present && present[HTML_OPTION_VALUE] &&
		value[HTML_OPTION_VALUE]) {
		if (!I_value) {
		     
		    StrAllocCopy(I_value, value[HTML_OPTION_VALUE]);
		    me->HiddenValue = TRUE;
		    TRANSLATE_AND_UNESCAPE_ENTITIES6(&I_value,
						     ATTR_CS_IN,
						     ATTR_CS_IN,
						     NO,
						     me->UsePlainSpace, me->HiddenValue);
		    me->HiddenValue = FALSE;
		}
		StrAllocCopy(me->LastOptionValue, I_value);
	    } else {
		StrAllocCopy(me->LastOptionValue, me->option.data);
	    }

	     
	    if (HTCurSelectGroupType == F_RADIO_TYPE &&
		LYSelectPopups &&
		fields_are_numbered()) {
		char marker[8];
		int opnum = HText_getOptionNum(me->text);

		if (opnum > 0 && opnum < 100000) {
		    sprintf(marker, "(%d)", opnum);
		    HTML_put_string(me, marker);
		    for (i = (int) strlen(marker); i < 5; ++i) {
			HTML_put_character(me, '_');
		    }
		}
	    }
	    FREE(I_value);
	}
	break;

    case HTML_TABLE:
	 
#ifdef EXP_NESTED_TABLES
	if (!nested_tables)
#endif
	{
	    HText_cancelStbl(me->text);
	}

	if (me->inA) {
	    SET_SKIP_STACK(HTML_A);
	    HTML_end_element(me, HTML_A, include);
	}
	if (me->Underline_Level > 0) {
	    SET_SKIP_STACK(HTML_U);
	    HTML_end_element(me, HTML_U, include);
	}
	me->inTABLE = TRUE;
	if (me->sp->style->id == ST_Preformatted) {
	    UPDATE_STYLE;
	    CHECK_ID(HTML_TABLE_ID);
	    break;
	}
	if (me->Division_Level < (MAX_NESTING - 1)) {
	    me->Division_Level++;
	} else {
	    CTRACE((tfp,
		    "HTML: ****** Maximum nesting of %d divisions/tables exceeded!\n",
		    MAX_NESTING));
	}
	if (present && present[HTML_TABLE_ALIGN] &&
	    non_empty(value[HTML_TABLE_ALIGN])) {
	    if (!strcasecomp(value[HTML_TABLE_ALIGN], "center")) {
		if (no_table_center) {
		    me->DivisionAlignments[me->Division_Level] = HT_LEFT;
		    change_paragraph_style(me, styles[HTML_DLEFT]);
		    UPDATE_STYLE;
		    me->current_default_alignment =
			styles[HTML_DLEFT]->alignment;
		} else {
		    me->DivisionAlignments[me->Division_Level] = HT_CENTER;
		    change_paragraph_style(me, styles[HTML_DCENTER]);
		    UPDATE_STYLE;
		    me->current_default_alignment =
			styles[HTML_DCENTER]->alignment;
		}

		stbl_align = HT_CENTER;

	    } else if (!strcasecomp(value[HTML_TABLE_ALIGN], "right")) {
		me->DivisionAlignments[me->Division_Level] = HT_RIGHT;
		change_paragraph_style(me, styles[HTML_DRIGHT]);
		UPDATE_STYLE;
		me->current_default_alignment = styles[HTML_DRIGHT]->alignment;
		stbl_align = HT_RIGHT;
	    } else {
		me->DivisionAlignments[me->Division_Level] = HT_LEFT;
		change_paragraph_style(me, styles[HTML_DLEFT]);
		UPDATE_STYLE;
		me->current_default_alignment = styles[HTML_DLEFT]->alignment;
		if (!strcasecomp(value[HTML_TABLE_ALIGN], "left") ||
		    !strcasecomp(value[HTML_TABLE_ALIGN], "justify"))
		    stbl_align = HT_LEFT;
	    }
	} else {
	    me->DivisionAlignments[me->Division_Level] = HT_LEFT;
	    change_paragraph_style(me, styles[HTML_DLEFT]);
	    UPDATE_STYLE;
	    me->current_default_alignment = styles[HTML_DLEFT]->alignment;
	     
	}
	CHECK_ID(HTML_TABLE_ID);
	HText_startStblTABLE(me->text, stbl_align);
	break;

    case HTML_TR:
	 
	if (me->inA) {
	    SET_SKIP_STACK(HTML_A);
	    HTML_end_element(me, HTML_A, include);
	}
	if (me->Underline_Level > 0) {
	    SET_SKIP_STACK(HTML_U);
	    HTML_end_element(me, HTML_U, include);
	}
	UPDATE_STYLE;
	if (!HText_LastLineEmpty(me->text, FALSE)) {
	    HText_setLastChar(me->text, ' ');	 
	    HText_appendCharacter(me->text, '\r');
	}
	me->in_word = NO;

	if (me->sp->style->id == ST_Preformatted) {
	    CHECK_ID(HTML_TR_ID);
	    me->inP = FALSE;
	    break;
	}
	if (LYoverride_default_alignment(me)) {
	    me->sp->style->alignment = styles[me->sp[0].tag_number]->alignment;
	} else if (me->List_Nesting_Level >= 0 ||
		   ((me->Division_Level < 0) &&
		    (me->sp->style->id == ST_Normal ||
		     me->sp->style->id == ST_Preformatted))) {
	    me->sp->style->alignment = HT_LEFT;
	} else {
	    me->sp->style->alignment = (short) me->current_default_alignment;
	}
	if (present && present[HTML_TR_ALIGN] && value[HTML_TR_ALIGN]) {
	    if (!strcasecomp(value[HTML_TR_ALIGN], "center") &&
		!(me->List_Nesting_Level >= 0 && !me->inP)) {
		if (no_table_center)
		    me->sp->style->alignment = HT_LEFT;
		else
		    me->sp->style->alignment = HT_CENTER;
		stbl_align = HT_CENTER;
	    } else if (!strcasecomp(value[HTML_TR_ALIGN], "right") &&
		       !(me->List_Nesting_Level >= 0 && !me->inP)) {
		me->sp->style->alignment = HT_RIGHT;
		stbl_align = HT_RIGHT;
	    } else if (!strcasecomp(value[HTML_TR_ALIGN], "left") ||
		       !strcasecomp(value[HTML_TR_ALIGN], "justify")) {
		me->sp->style->alignment = HT_LEFT;
		stbl_align = HT_LEFT;
	    }
	}

	CHECK_ID(HTML_TR_ID);
	me->inP = FALSE;
	HText_startStblTR(me->text, stbl_align);
	break;

    case HTML_THEAD:
    case HTML_TFOOT:
    case HTML_TBODY:
	HText_endStblTR(me->text);
	 
	if (me->inA) {
	    SET_SKIP_STACK(HTML_A);
	    HTML_end_element(me, HTML_A, include);
	}
	if (me->Underline_Level > 0) {
	    SET_SKIP_STACK(HTML_U);
	    HTML_end_element(me, HTML_U, include);
	}
	UPDATE_STYLE;
	if (me->inTABLE) {
	    if (present && present[HTML_TR_ALIGN] && value[HTML_TR_ALIGN]) {
		if (!strcasecomp(value[HTML_TR_ALIGN], "center")) {
		    stbl_align = HT_CENTER;
		} else if (!strcasecomp(value[HTML_TR_ALIGN], "right")) {
		    stbl_align = HT_RIGHT;
		} else if (!strcasecomp(value[HTML_TR_ALIGN], "left") ||
			   !strcasecomp(value[HTML_TR_ALIGN], "justify")) {
		    stbl_align = HT_LEFT;
		}
	    }
	    HText_startStblRowGroup(me->text, stbl_align);
	}
	CHECK_ID(HTML_TR_ID);
	break;

    case HTML_COL:
    case HTML_COLGROUP:
	 
	if (me->inA) {
	    SET_SKIP_STACK(HTML_A);
	    HTML_end_element(me, HTML_A, include);
	}
	if (me->Underline_Level > 0) {
	    SET_SKIP_STACK(HTML_U);
	    HTML_end_element(me, HTML_U, include);
	}
	UPDATE_STYLE;
	if (me->inTABLE) {
	    int span = 1;

	    if (present && present[HTML_COL_SPAN] &&
		value[HTML_COL_SPAN] &&
		isdigit(UCH(*value[HTML_COL_SPAN])))
		span = atoi(value[HTML_COL_SPAN]);
	    if (present && present[HTML_COL_ALIGN] && value[HTML_COL_ALIGN]) {
		if (!strcasecomp(value[HTML_COL_ALIGN], "center")) {
		    stbl_align = HT_CENTER;
		} else if (!strcasecomp(value[HTML_COL_ALIGN], "right")) {
		    stbl_align = HT_RIGHT;
		} else if (!strcasecomp(value[HTML_COL_ALIGN], "left") ||
			   !strcasecomp(value[HTML_COL_ALIGN], "justify")) {
		    stbl_align = HT_LEFT;
		}
	    }
	    HText_startStblCOL(me->text, span, stbl_align,
			       (BOOL) (ElementNumber == HTML_COLGROUP));
	}
	CHECK_ID(HTML_COL_ID);
	break;

    case HTML_TH:
    case HTML_TD:
	if (me->inA) {
	    SET_SKIP_STACK(HTML_A);
	    HTML_end_element(me, HTML_A, include);
	}
	if (me->Underline_Level > 0) {
	    SET_SKIP_STACK(HTML_U);
	    HTML_end_element(me, HTML_U, include);
	}
	UPDATE_STYLE;
	CHECK_ID(HTML_TD_ID);
	 
	HTML_put_character(me, ' ');
	{
	    int colspan = 1, rowspan = 1;

	    if (present && present[HTML_TD_COLSPAN] &&
		value[HTML_TD_COLSPAN] &&
		isdigit(UCH(*value[HTML_TD_COLSPAN])))
		colspan = atoi(value[HTML_TD_COLSPAN]);
	    if (present && present[HTML_TD_ROWSPAN] &&
		value[HTML_TD_ROWSPAN] &&
		isdigit(UCH(*value[HTML_TD_ROWSPAN])))
		rowspan = atoi(value[HTML_TD_ROWSPAN]);
	    if (present && present[HTML_TD_ALIGN] && value[HTML_TD_ALIGN]) {
		if (!strcasecomp(value[HTML_TD_ALIGN], "center")) {
		    stbl_align = HT_CENTER;
		} else if (!strcasecomp(value[HTML_TD_ALIGN], "right")) {
		    stbl_align = HT_RIGHT;
		} else if (!strcasecomp(value[HTML_TD_ALIGN], "left") ||
			   !strcasecomp(value[HTML_TD_ALIGN], "justify")) {
		    stbl_align = HT_LEFT;
		}
	    }
	    HText_startStblTD(me->text, colspan, rowspan, stbl_align,
			      (BOOL) (ElementNumber == HTML_TH));
	}
	me->in_word = NO;
	break;

    case HTML_MATH:
	 
	HTChunkClear(&me->math);
	CHECK_ID(HTML_GEN_ID);
	break;

    default:
	break;

    }				 

    if (ElementNumber >= HTML_ELEMENTS ||
	HTML_dtd.tags[ElementNumber].contents != SGML_EMPTY) {
	if (me->skip_stack > 0) {
	    CTRACE((tfp,
		    "HTML:begin_element: internal call (level %d), leaving on stack - `%s'\n",
		    me->skip_stack, NONNULL(GetHTStyleName(me->sp->style))));
	    me->skip_stack--;
	    return status;
	}
	if (me->sp == me->stack) {
	    if (me->stack_overrun == FALSE) {
		HTAlert(HTML_STACK_OVERRUN);
		CTRACE((tfp,
			"HTML: ****** Maximum nesting of %d tags exceeded!\n",
			MAX_NESTING));
		me->stack_overrun = TRUE;
	    }
	    return HT_ERROR;
	}

	CTRACE((tfp,
		"HTML:begin_element[%d]: adding style to stack - %s (%s)\n",
		(int) STACKLEVEL(me),
		NONNULL(GetHTStyleName(me->new_style)),
		HTML_dtd.tags[ElementNumber].name));
	(me->sp)--;
	me->sp[0].style = me->new_style;	 
	me->sp[0].tag_number = ElementNumber;
#ifdef USE_JUSTIFY_ELTS
	if (wait_for_this_stacked_elt < 0 &&
	    HTML_dtd.tags[ElementNumber].can_justify == FALSE)
	    wait_for_this_stacked_elt = (int) (me->stack - me->sp) + MAX_NESTING;
#endif
    }
#ifdef USE_JUSTIFY_ELTS
    if (in_DT && ElementNumber == HTML_DD)
	in_DT = FALSE;
    else if (ElementNumber == HTML_DT)
	in_DT = TRUE;
#endif

#if defined(USE_COLOR_STYLE)
 

    if (ReallyEmptyTagNum(element_number)) {
	CTRACE2(TRACE_STYLE,
		(tfp, "STYLE.begin_element:ending \"EMPTY\" element style\n"));
	HText_characterStyle(me->text, HCODE_TO_STACK_OFF(hcode), STACK_OFF);

#  if !OMIT_SCN_KEEPING
	FastTrimColorClass(HTML_dtd.tags[element_number].name,
			   HTML_dtd.tags[element_number].name_len,
			   Style_className,
			   &Style_className_end, &hcode);
#  endif
    }
#endif  
    return status;
}
