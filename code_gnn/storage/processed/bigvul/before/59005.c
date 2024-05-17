static int HTML_end_element(HTStructured * me, int element_number,
			    char **include)
{
    static char empty[1];

    int i = 0;
    int status = HT_OK;
    char *temp = NULL, *cp = NULL;
    BOOL BreakFlag = FALSE;
    BOOL intern_flag = FALSE;

#ifdef USE_COLOR_STYLE
    BOOL skip_stack_requested = FALSE;
#endif
    EMIT_IFDEF_USE_JUSTIFY_ELTS(BOOL reached_awaited_stacked_elt = FALSE);

#ifdef USE_PRETTYSRC
    if (psrc_view && !sgml_in_psrc_was_initialized) {
	if (!psrc_nested_call) {
	    HTTag *tag = &HTML_dtd.tags[element_number];
	    char buf[200];
	    int tag_charset = 0;

	    psrc_nested_call = TRUE;
	    PSRCSTART(abracket);
	    PUTS("</");
	    PSRCSTOP(abracket);
	    PSRCSTART(tag);
	    if (tagname_transform != 0)
		PUTS(tag->name);
	    else {
		LYStrNCpy(buf, tag->name, sizeof(buf) - 1);
		LYLowerCase(buf);
		PUTS(buf);
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

    if ((me->sp >= (me->stack + MAX_NESTING - 1) ||
	 element_number != me->sp[0].tag_number) &&
	HTML_dtd.tags[element_number].contents != SGML_EMPTY) {
	CTRACE((tfp,
		"HTML: end of element %s when expecting end of %s\n",
		HTML_dtd.tags[element_number].name,
		(me->sp == me->stack + MAX_NESTING - 1) ? "none" :
		(me->sp->tag_number < 0) ? "*invalid tag*" :
		(me->sp->tag_number >= HTML_ELEMENTS) ? "special tag" :
		HTML_dtd.tags[me->sp->tag_number].name));
    }

     
    if (LYMapsOnly) {
	if (!(element_number == HTML_MAP || element_number == HTML_AREA ||
	      element_number == HTML_OBJECT)) {
	    return HT_OK;
	}
    }

     
    if (HTML_dtd.tags[element_number].contents != SGML_EMPTY) {
#ifdef USE_COLOR_STYLE
	skip_stack_requested = (BOOL) (me->skip_stack > 0);
#endif
	if ((element_number != me->sp[0].tag_number) &&
	    me->skip_stack <= 0 &&
	    HTML_dtd.tags[HTML_LH].contents != SGML_EMPTY &&
	    (me->sp[0].tag_number == HTML_UL ||
	     me->sp[0].tag_number == HTML_OL ||
	     me->sp[0].tag_number == HTML_MENU ||
	     me->sp[0].tag_number == HTML_DIR ||
	     me->sp[0].tag_number == HTML_LI) &&
	    (element_number == HTML_H1 ||
	     element_number == HTML_H2 ||
	     element_number == HTML_H3 ||
	     element_number == HTML_H4 ||
	     element_number == HTML_H5 ||
	     element_number == HTML_H6)) {
	     
	    BreakFlag = TRUE;
	}
	if (me->skip_stack == 0 && element_number == HTML_OBJECT &&
	    me->sp[0].tag_number == HTML_OBJECT_M &&
	    (me->sp < (me->stack + MAX_NESTING - 1)))
	    me->sp[0].tag_number = HTML_OBJECT;
	if (me->skip_stack > 0) {
	    CTRACE2(TRACE_STYLE,
		    (tfp,
		     "HTML:end_element: Internal call (level %d), leaving on stack - %s\n",
		     me->skip_stack, NONNULL(GetHTStyleName(me->sp->style))));
	    me->skip_stack--;
	} else if (element_number == HTML_OBJECT &&
		   me->sp[0].tag_number != HTML_OBJECT &&
		   me->sp[0].tag_number != HTML_OBJECT_M &&
		   me->objects_mixed_open > 0 &&
		   !(me->objects_figged_open > 0 &&
		     me->sp[0].tag_number == HTML_FIG)) {
	     
	    CTRACE2(TRACE_STYLE,
		    (tfp, "HTML:end_element[%d]: %s (level %d), %s - %s\n",
		     (int) STACKLEVEL(me),
		     "Special OBJECT handling", me->objects_mixed_open,
		     "leaving on stack",
		     NONNULL(GetHTStyleName(me->sp->style))));
	    me->objects_mixed_open--;
	} else if (me->stack_overrun == TRUE &&
		   element_number != me->sp[0].tag_number) {
	     
	    return HT_OK;	 
	} else if (element_number == HTML_SELECT &&
		   me->sp[0].tag_number != HTML_SELECT) {
	     
	    return HT_OK;
	} else if ((element_number != me->sp[0].tag_number) &&
		   HTML_dtd.tags[HTML_LH].contents == SGML_EMPTY &&
		   (me->sp[0].tag_number == HTML_UL ||
		    me->sp[0].tag_number == HTML_OL ||
		    me->sp[0].tag_number == HTML_MENU ||
		    me->sp[0].tag_number == HTML_DIR ||
		    me->sp[0].tag_number == HTML_LI) &&
		   (element_number == HTML_H1 ||
		    element_number == HTML_H2 ||
		    element_number == HTML_H3 ||
		    element_number == HTML_H4 ||
		    element_number == HTML_H5 ||
		    element_number == HTML_H6)) {
	     
	    return HT_OK;
	} else if (me->sp < (me->stack + MAX_NESTING - 1)) {
#ifdef USE_JUSTIFY_ELTS
	    if (wait_for_this_stacked_elt == me->stack - me->sp + MAX_NESTING)
		reached_awaited_stacked_elt = TRUE;
#endif
	    if (element_number == HTML_OBJECT) {
		if (me->sp[0].tag_number == HTML_FIG &&
		    me->objects_figged_open > 0) {
		     
		    CTRACE2(TRACE_STYLE,
			    (tfp,
			     "HTML:end_element[%d]: %s (level %d), %s - %s\n",
			     (int) STACKLEVEL(me),
			     "Special OBJECT->FIG handling",
			     me->objects_figged_open,
			     "treating as end FIG",
			     NONNULL(GetHTStyleName(me->sp->style))));
		    me->objects_figged_open--;
		    element_number = HTML_FIG;
		}
	    }
	    (me->sp)++;
	    CTRACE2(TRACE_STYLE,
		    (tfp,
		     "HTML:end_element[%d]: Popped style off stack - %s\n",
		     (int) STACKLEVEL(me),
		     NONNULL(GetHTStyleName(me->sp->style))));
	} else {
	    CTRACE2(TRACE_STYLE, (tfp,
				  "Stack underflow error!  Tried to pop off more styles than exist in stack\n"));
	}
    }
    if (BreakFlag == TRUE) {
#ifdef USE_JUSTIFY_ELTS
	if (reached_awaited_stacked_elt)
	    wait_for_this_stacked_elt = -1;
#endif
	return HT_OK;		 
    }

     
    if (me->inTEXTAREA && element_number != HTML_TEXTAREA) {
	if (LYBadHTML(me)) {
	    LYShowBadHTML("Bad HTML: Missing TEXTAREA end tag\n");
	}
    }

    if (!me->text && !LYMapsOnly) {
	UPDATE_STYLE;
    }

     
    switch (element_number) {

    case HTML_HTML:
	if (me->inA || me->inSELECT || me->inTEXTAREA) {
	    if (LYBadHTML(me)) {
		char *msg = NULL;

		HTSprintf0(&msg,
			   "Bad HTML: %s%s%s%s%s not closed before HTML end tag *****\n",
			   me->inSELECT ? "SELECT" : "",
			   (me->inSELECT && me->inTEXTAREA) ? ", " : "",
			   me->inTEXTAREA ? "TEXTAREA" : "",
			   (((me->inSELECT || me->inTEXTAREA) && me->inA)
			    ? ", "
			    : ""),
			   me->inA ? "A" : "");
		LYShowBadHTML(msg);
		FREE(msg);
	    }
	}
	break;

    case HTML_HEAD:
	if (me->inBASE &&
	    (LYIsUIPage3(me->node_anchor->address, UIP_LIST_PAGE, 0) ||
	     LYIsUIPage3(me->node_anchor->address, UIP_ADDRLIST_PAGE, 0))) {
	     
	    StrAllocCopy(me->node_anchor->content_base, me->base_href);
	}
	if (HText_hasToolbar(me->text))
	    HText_appendParagraph(me->text);
	break;

    case HTML_TITLE:
	HTChunkTerminate(&me->title);
	HTAnchor_setTitle(me->node_anchor, me->title.data);
	HTChunkClear(&me->title);
	 
	if (non_empty(me->node_anchor->bookmark)) {
	    if ((LYMultiBookmarks != MBM_OFF) ||
		(non_empty(bookmark_page) &&
		 strcmp(me->node_anchor->bookmark, bookmark_page))) {
		if (!include)
		    include = &me->xinclude;
		for (i = 0; i <= MBM_V_MAXFILES; i++) {
		    if (MBM_A_subbookmark[i] &&
			!strcmp(MBM_A_subbookmark[i],
				me->node_anchor->bookmark)) {
			StrAllocCat(*include, "<H2><EM>");
			StrAllocCat(*include, gettext("Description:"));
			StrAllocCat(*include, "</EM> ");
			StrAllocCopy(temp,
				     ((MBM_A_subdescript[i] &&
				       *MBM_A_subdescript[i]) ?
				      MBM_A_subdescript[i] : gettext("(none)")));
			LYEntify(&temp, TRUE);
			StrAllocCat(*include, temp);
			StrAllocCat(*include, "<BR><EM>&nbsp;&nbsp;&nbsp;");
			StrAllocCat(*include, gettext("Filepath:"));
			StrAllocCat(*include, "</EM> ");
			StrAllocCopy(temp,
				     ((MBM_A_subbookmark[i] &&
				       *MBM_A_subbookmark[i])
				      ? MBM_A_subbookmark[i]
				      : gettext("(unknown)")));
			LYEntify(&temp, TRUE);
			StrAllocCat(*include, temp);
			FREE(temp);
			StrAllocCat(*include, "</H2>");
			break;
		    }
		}
	    }
	}
	break;

    case HTML_STYLE:
	 
	HTChunkTerminate(&me->style_block);
	CTRACE2(TRACE_STYLE,
		(tfp, "HTML: STYLE content =\n%s\n",
		 me->style_block.data));
	HTChunkClear(&me->style_block);
	break;

    case HTML_SCRIPT:
	 
	HTChunkTerminate(&me->script);
	CTRACE((tfp, "HTML: SCRIPT content =\n%s\n",
		me->script.data));
	HTChunkClear(&me->script);
	break;

    case HTML_BODY:
	if (me->inA || me->inSELECT || me->inTEXTAREA) {
	    if (LYBadHTML(me)) {
		char *msg = NULL;

		HTSprintf0(&msg,
			   "Bad HTML: %s%s%s%s%s not closed before BODY end tag *****\n",
			   me->inSELECT ? "SELECT" : "",
			   (me->inSELECT && me->inTEXTAREA) ? ", " : "",
			   me->inTEXTAREA ? "TEXTAREA" : "",
			   (((me->inSELECT || me->inTEXTAREA) && me->inA)
			    ? ", "
			    : ""),
			   me->inA ? "A" : "");
		LYShowBadHTML(msg);
		FREE(msg);
	    }
	}
	break;

    case HTML_FRAMESET:
	change_paragraph_style(me, me->sp->style);	 
	break;

    case HTML_NOFRAMES:
    case HTML_IFRAME:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	change_paragraph_style(me, me->sp->style);	 
	break;

    case HTML_BANNER:
    case HTML_MARQUEE:
    case HTML_BLOCKQUOTE:
    case HTML_BQ:
    case HTML_ADDRESS:
	 
	change_paragraph_style(me, me->sp->style);
	UPDATE_STYLE;
	if (me->sp->tag_number == element_number)
	    LYEnsureDoubleSpace(me);
	if (me->List_Nesting_Level >= 0)
	    HText_NegateLineOne(me->text);
	break;

    case HTML_CENTER:
    case HTML_DIV:
	if (me->Division_Level >= 0)
	    me->Division_Level--;
	if (me->Division_Level >= 0) {
	    if (me->sp->style->alignment !=
		me->DivisionAlignments[me->Division_Level]) {
		if (me->inP)
		    LYEnsureSingleSpace(me);
		me->sp->style->alignment =
		    me->DivisionAlignments[me->Division_Level];
	    }
	}
	change_paragraph_style(me, me->sp->style);
	if (me->style_change) {
	    actually_set_style(me);
	    if (me->List_Nesting_Level >= 0)
		HText_NegateLineOne(me->text);
	} else if (me->inP)
	    LYEnsureSingleSpace(me);
	me->current_default_alignment = me->sp->style->alignment;
	break;

    case HTML_H1:		 
    case HTML_H2:
    case HTML_H3:
    case HTML_H4:
    case HTML_H5:
    case HTML_H6:
	if (me->Division_Level >= 0) {
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
	change_paragraph_style(me, me->sp->style);
	UPDATE_STYLE;
	if (styles[element_number]->font & HT_BOLD) {
	    if (me->inBoldA == FALSE && me->inBoldH == TRUE) {
		HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
	    }
	    me->inBoldH = FALSE;
	}
	if (me->List_Nesting_Level >= 0)
	    HText_NegateLineOne(me->text);
	if (me->Underline_Level > 0 && me->inUnderline == FALSE) {
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	    me->inUnderline = TRUE;
	}
	break;

    case HTML_P:
	LYHandlePlike(me,
		      (const BOOL *) 0, (STRING2PTR) 0,
		      include, 0,
		      FALSE);
	break;

    case HTML_FONT:
	me->inFONT = FALSE;
	break;

    case HTML_B:		 
    case HTML_BLINK:
    case HTML_I:
    case HTML_U:

    case HTML_CITE:		 
    case HTML_EM:
    case HTML_STRONG:
	 
	if (me->Underline_Level <= 0)
	    break;

	 
	me->Underline_Level--;
	if (me->inUnderline && me->Underline_Level < 1) {
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	    me->inUnderline = FALSE;
	    CTRACE((tfp, "Ending underline\n"));
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
    case HTML_SUP:
    case HTML_TT:
    case HTML_VAR:
	break;

    case HTML_SUB:
	HText_appendCharacter(me->text, ']');
	break;

    case HTML_DEL:
    case HTML_S:
    case HTML_STRIKE:
	HTML_put_character(me, ' ');
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	HTML_put_string(me, ":DEL]");
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	HTML_put_character(me, ' ');
	me->in_word = NO;
	break;

    case HTML_INS:
	HTML_put_character(me, ' ');
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	HTML_put_string(me, ":INS]");
	if (me->inUnderline == FALSE)
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	HTML_put_character(me, ' ');
	me->in_word = NO;
	break;

    case HTML_Q:
	if (me->Quote_Level > 0)
	    me->Quote_Level--;
	 
	if (!(me->Quote_Level & 1))
	    HTML_put_character(me, '"');
	else
	    HTML_put_character(me, '\'');
	break;

    case HTML_PRE:		 
	 
	HText_appendCharacter(me->text, '\n');
	me->inPRE = FALSE;
	 
    case HTML_LISTING:		 
	 
    case HTML_XMP:
	 
    case HTML_PLAINTEXT:
	if (me->comment_start)
	    HText_appendText(me->text, me->comment_start);
	change_paragraph_style(me, me->sp->style);	 
	if (me->List_Nesting_Level >= 0) {
	    UPDATE_STYLE;
	    HText_NegateLineOne(me->text);
	}
	break;

    case HTML_NOTE:
    case HTML_FN:
	change_paragraph_style(me, me->sp->style);	 
	UPDATE_STYLE;
	if (me->sp->tag_number == element_number)
	    LYEnsureDoubleSpace(me);
	if (me->List_Nesting_Level >= 0)
	    HText_NegateLineOne(me->text);
	me->inLABEL = FALSE;
	break;

    case HTML_OL:
	me->OL_Counter[me->List_Nesting_Level < 11 ?
		       me->List_Nesting_Level : 11] = OL_VOID;
	 
    case HTML_DL:
	 
    case HTML_UL:
	 
    case HTML_MENU:
	 
    case HTML_DIR:
	me->List_Nesting_Level--;
	CTRACE((tfp, "HTML_end_element: Reducing List Nesting Level to %d\n",
		me->List_Nesting_Level));
#ifdef USE_JUSTIFY_ELTS
	if (element_number == HTML_DL)
	    in_DT = FALSE;	 
#endif
	change_paragraph_style(me, me->sp->style);	 
	UPDATE_STYLE;
	if (me->List_Nesting_Level >= 0)
	    LYEnsureSingleSpace(me);
	break;

    case HTML_SPAN:
	 
	break;

    case HTML_BDO:
	 
	break;

    case HTML_A:
	 
	if (me->inA == FALSE)
	    break;
	 
	me->inA = FALSE;
#ifdef MARK_HIDDEN_LINKS
	if (non_empty(hidden_link_marker) &&
	    HText_isAnchorBlank(me->text, me->CurrentANum)) {
	    HText_appendText(me->text, hidden_link_marker);
	}
#endif
	UPDATE_STYLE;
	if (me->inBoldA == TRUE && me->inBoldH == FALSE)
	    HText_appendCharacter(me->text, LY_BOLD_END_CHAR);
	HText_endAnchor(me->text, me->CurrentANum);
	me->CurrentANum = 0;
	me->inBoldA = FALSE;
	if (me->Underline_Level > 0 && me->inUnderline == FALSE) {
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	    me->inUnderline = TRUE;
	}
	break;

    case HTML_MAP:
	FREE(me->map_address);
	break;

    case HTML_BODYTEXT:
	 
	change_paragraph_style(me, me->sp->style);	 
	break;

    case HTML_TEXTFLOW:
	 
	change_paragraph_style(me, me->sp->style);	 
	break;

    case HTML_FIG:
	LYHandleFIG(me, NULL, NULL,
		    0,
		    0,
		    NULL,
		    NULL, NO, FALSE, &intern_flag);
	break;

    case HTML_OBJECT:
	 
	{
	    int s = 0, e = 0;
	    char *start = NULL, *first_end = NULL, *last_end = NULL;
	    char *first_map = NULL, *last_map = NULL;
	    BOOL have_param = FALSE;
	    char *data = NULL;

	    HTChunkTerminate(&me->object);
	    data = me->object.data;
	    while ((cp = StrChr(data, '<')) != NULL) {
		 
		if (!StrNCmp(cp, "<!--", 4)) {
		    data = LYFindEndOfComment(cp);
		    cp = data;
		} else if (s == 0 && !strncasecomp(cp, "<PARAM", 6) &&
			   !IsNmChar(cp[6])) {
		    have_param = TRUE;
		} else if (!strncasecomp(cp, "<OBJECT", 7) &&
			   !IsNmChar(cp[7])) {
		    if (s == 0)
			start = cp;
		    s++;
		} else if (!strncasecomp(cp, "</OBJECT", 8) &&
			   !IsNmChar(cp[8])) {
		    if (e == 0)
			first_end = cp;
		    last_end = cp;
		    e++;
		} else if (!strncasecomp(cp, "<MAP", 4) &&
			   !IsNmChar(cp[4])) {
		    if (!first_map)
			first_map = cp;
		    last_map = cp;
		} else if (!strncasecomp(cp, "</MAP", 5) &&
			   !IsNmChar(cp[5])) {
		    last_map = cp;
		}
		data = ++cp;
	    }
	    if (s < e) {
		 
		if (LYBadHTML(me)) {
		    char *msg = NULL;

		    HTSprintf0(&msg,
			       "Bad HTML: Unmatched OBJECT start and end tags.  Discarding content:\n%s\n",
			       me->object.data);
		    LYShowBadHTML(msg);
		    FREE(msg);
		}
		goto End_Object;
	    }
	    if (s > e) {
		if (!me->object_declare && !me->object_name &&
		    !(me->object_shapes && !LYMapsOnly) &&
		    !(me->object_usemap != NULL && !LYMapsOnly) &&
		    !(clickable_images && !LYMapsOnly &&
		      me->object_data != NULL &&
		      !have_param &&
		      me->object_classid == NULL &&
		      me->object_codebase == NULL &&
		      me->object_codetype == NULL)) {
		     
		    CTRACE((tfp, "%s:\n%s\n",
			    "HTML: Nested OBJECT tags.  Recycling incomplete contents",
			    me->object.data));
		    status = HT_PARSER_OTHER_CONTENT;
		    me->object.size--;
		    HTChunkPuts(&me->object, "</OBJECT>");
		    if (!include)	 
			include = &me->xinclude;
		    StrnAllocCat(*include, me->object.data, (size_t) me->object.size);
		    clear_objectdata(me);
		     
		    HTML_start_element(me, HTML_OBJECT, NULL, NULL,
				       me->tag_charset, include);
		    break;
		}
		 
		CTRACE((tfp, "HTML: Nested OBJECT tags.  Recycling.\n"));
		status = HT_PARSER_REOPEN_ELT;
		me->object.size--;
		HTChunkPuts(&me->object, "</OBJECT>");
		if (!LYMapsOnly)
		    change_paragraph_style(me, me->sp->style);
		break;
	    }

	     
	    CTRACE((tfp, "HTML:OBJECT content:\n%s\n", me->object.data));

	     
	    if (me->object_declare == TRUE) {
		if (non_empty(me->object_id) && !LYMapsOnly)
		    LYHandleID(me, me->object_id);
		CTRACE((tfp, "HTML: DECLAREd OBJECT.  Ignoring!\n"));
		goto End_Object;
	    }

	     
	    if (me->object_name != NULL && !LYMapsOnly) {
		if (non_empty(me->object_id))
		    LYHandleID(me, me->object_id);
		CTRACE((tfp, "HTML: NAMEd OBJECT.  Ignoring!\n"));
		goto End_Object;
	    }

	     
	    if (s > 0) {
		if (start != NULL &&
		    first_end != NULL && first_end > start) {
		     
		    if (LYMapsOnly) {
			 
			if (LYMapsOnly && (!last_map || last_map < first_end))
			    *first_end = '\0';
			else
			    e = 0;
			data = NULL;
			if (LYMapsOnly && (!first_map || first_map > start))
			    StrAllocCopy(data, start);
			else
			    StrAllocCopy(data, me->object.data);
			if (e > 0) {
			    for (i = e; i > 0; i--) {
				StrAllocCat(data, "</OBJECT>");
			    }
			}
			if (!include)	 
			    include = &me->xinclude;
			StrAllocCat(*include, data);
			CTRACE((tfp, "HTML: Recycling nested OBJECT%s.\n",
				(s > 1) ? "s" : ""));
			FREE(data);
			goto End_Object;
		    }
		} else {
		    if (LYBadHTML(me)) {
			LYShowBadHTML("Bad HTML: Unmatched OBJECT start and end tags.  Discarding content.\n");
		    }
		    goto End_Object;
		}
	    }

	     
	    if (me->object_shapes == TRUE && !LYMapsOnly) {
		CTRACE((tfp, "HTML: OBJECT has SHAPES.  Converting to FIG.\n"));
		if (!include)	 
		    include = &me->xinclude;
		StrAllocCat(*include, "<FIG ISOBJECT IMAGEMAP");
		if (me->object_ismap == TRUE)
		    StrAllocCat(*include, " IMAGEMAP");
		if (me->object_id != NULL) {
		    StrAllocCat(*include, " ID=\"");
		    StrAllocCat(*include, me->object_id);
		    StrAllocCat(*include, "\"");
		}
		if (me->object_data != NULL &&
		    me->object_classid == NULL) {
		    StrAllocCat(*include, " SRC=\"");
		    StrAllocCat(*include, me->object_data);
		    StrAllocCat(*include, "\"");
		}
		StrAllocCat(*include, ">");
		me->object.size--;
		HTChunkPuts(&me->object, "</FIG>");
		HTChunkTerminate(&me->object);
		StrAllocCat(*include, me->object.data);
		goto End_Object;
	    }

	     
	    if (me->object_usemap != NULL && !LYMapsOnly) {
		CTRACE((tfp, "HTML: OBJECT has USEMAP.  Converting to IMG.\n"));

		if (!include)	 
		    include = &me->xinclude;
		StrAllocCat(*include, "<IMG ISOBJECT");
		if (me->object_id != NULL) {
		     
		    StrAllocCat(*include, " ID=\"");
		    StrAllocCat(*include, me->object_id);
		    StrAllocCat(*include, "\"");
		}
		if (me->object_data != NULL &&
		    me->object_classid == NULL) {
		     
		    StrAllocCat(*include, " SRC=\"");
		    StrAllocCat(*include, me->object_data);
		    StrAllocCat(*include, "\"");
		}
		if (me->object_title != NULL) {
		     
		    StrAllocCat(*include, " TITLE=\"");
		    StrAllocCat(*include, me->object_title);
		    StrAllocCat(*include, "\" ALT=\"");
		    StrAllocCat(*include, me->object_title);
		    StrAllocCat(*include, "\"");
		}
		 
		if (me->object_usemap != NULL) {
		    StrAllocCat(*include, " USEMAP=\"");
		    StrAllocCat(*include, me->object_usemap);
		    if (me->object_ismap == TRUE)
			StrAllocCat(*include, "\" ISMAP>");
		    else
			StrAllocCat(*include, "\">");
		} else {
		    StrAllocCat(*include, ">");
		}
		 
		if (first_map) {
		    if (s == 0) {
			StrAllocCat(*include, me->object.data);
			CTRACE((tfp,
				"HTML: MAP found, recycling object contents.\n"));
			goto End_Object;
		    }
		     
		    data = NULL;
		    if (last_map < start) {
			*start = '\0';
			i = 0;
		    } else if (last_map < first_end) {
			*first_end = '\0';
			i = e;
		    } else if (last_map < last_end) {
			*last_end = '\0';
			i = 1;
		    } else {
			i = 0;
		    }
		    if (first_map > last_end) {
			 
			StrAllocCopy(data, "<OBJECT><");
			StrAllocCat(data, last_end + 1);
			i = 0;
		    } else if (first_map > start) {
			StrAllocCopy(data, start);
		    } else {
			StrAllocCopy(data, me->object.data);
		    }
		    for (; i > 0; i--) {
			StrAllocCat(data, "</OBJECT>");
		    }
		    CTRACE((tfp, "%s:\n%s\n",
			    "HTML: MAP and nested OBJECT tags.  Recycling parts",
			    data));
		    StrAllocCat(*include, data);
		    FREE(data);
		}
		goto End_Object;
	    }

	     
	    if (non_empty(me->object_id) && !LYMapsOnly)
		LYHandleID(me, me->object_id);

	     
	    if (me->object.size > 1) {
		if (!include)	 
		    include = &me->xinclude;
		StrAllocCat(*include, me->object.data);
	    }

	     
	    if (clickable_images) {
		if (!LYMapsOnly &&
		    me->object_data != NULL &&
		    !have_param &&
		    me->object_classid == NULL &&
		    me->object_codebase == NULL &&
		    me->object_codetype == NULL) {
		     
		    if (!include)	 
			include = &me->xinclude;
		    if (me->inA)
			StrAllocCat(*include, "</A>");
		    StrAllocCat(*include, " -<A HREF=\"");
		    StrAllocCat(*include, me->object_data);
		    StrAllocCat(*include, "\">");
		    if ((me->object_type != NULL) &&
			!strncasecomp(me->object_type, "image/", 6)) {
			StrAllocCat(*include, "(IMAGE)");
		    } else {
			StrAllocCat(*include, "(OBJECT)");
		    }
		    StrAllocCat(*include, "</A> ");
		}
	    }
	}

	 
      End_Object:
	clear_objectdata(me);

	if (!LYMapsOnly)
	    change_paragraph_style(me, me->sp->style);	 
	break;

    case HTML_APPLET:
	if (me->inAPPLETwithP) {
	    LYEnsureDoubleSpace(me);
	} else {
	    HTML_put_character(me, ' ');	 
	}
	LYResetParagraphAlignment(me);
	me->inAPPLETwithP = FALSE;
	me->inAPPLET = FALSE;
	change_paragraph_style(me, me->sp->style);	 
	break;

    case HTML_CAPTION:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	me->inCAPTION = FALSE;
	change_paragraph_style(me, me->sp->style);	 
	me->inLABEL = FALSE;
	break;

    case HTML_CREDIT:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	me->inCREDIT = FALSE;
	change_paragraph_style(me, me->sp->style);	 
	me->inLABEL = FALSE;
	break;

    case HTML_FORM:
	 
	if (!me->inFORM) {
	    if (LYBadHTML(me)) {
		LYShowBadHTML("Bad HTML: Unmatched FORM end tag\n");
	    }
	}
	EMIT_IFDEF_USE_JUSTIFY_ELTS(form_in_htext = FALSE);

	 
	if (me->inSELECT) {
	    if (LYBadHTML(me)) {
		LYShowBadHTML("Bad HTML: Open SELECT at FORM end. Faking SELECT end tag. *****\n");
	    }
	    if (me->sp->tag_number != HTML_SELECT) {
		SET_SKIP_STACK(HTML_SELECT);
	    }
	    HTML_end_element(me, HTML_SELECT, include);
	}

	 
	me->inFORM = FALSE;

	HText_endForm(me->text);
	 
	if (!(me->List_Nesting_Level >= 0 && !me->inP))
	    LYEnsureSingleSpace(me);
	break;

    case HTML_FIELDSET:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	change_paragraph_style(me, me->sp->style);	 
	break;

    case HTML_LEGEND:
	LYEnsureDoubleSpace(me);
	LYResetParagraphAlignment(me);
	change_paragraph_style(me, me->sp->style);	 
	break;

    case HTML_LABEL:
	break;

    case HTML_BUTTON:
	break;

    case HTML_TEXTAREA:
	{
	    InputFieldData I;
	    int chars;
	    char *data;

	     
	    if (!me->inTEXTAREA) {
		if (LYBadHTML(me)) {
		    LYShowBadHTML("Bad HTML: Unmatched TEXTAREA end tag\n");
		}
		break;
	    }

	     
	    me->inTEXTAREA = FALSE;

	     
	    memset(&I, 0, sizeof(I));
	    I.value_cs = current_char_set;

	    UPDATE_STYLE;
	     
	    HTML_put_character(me, ' ');
	    me->in_word = NO;
	     
	    HText_appendCharacter(me->text, '\r');

	     
	    HTChunkTerminate(&me->textarea);
	    FREE(temp);

	    I.type = "textarea";
	    I.size = me->textarea_cols;
	    I.name = me->textarea_name;
	    I.name_cs = me->textarea_name_cs;
	    I.accept_cs = me->textarea_accept_cs;
	    me->textarea_accept_cs = NULL;
	    I.disabled = me->textarea_disabled;
	    I.readonly = me->textarea_readonly;
	    I.id = me->textarea_id;

	     
	    me->UsePlainSpace = TRUE;

	    if (HTML_dtd.tags[element_number].contents == SGML_LITTERAL) {
		TRANSLATE_AND_UNESCAPE_ENTITIES6(&me->textarea.data,
						 me->UCLYhndl,
						 current_char_set,
						 NO,
						 me->UsePlainSpace, me->HiddenValue);
	    } else {
		 
		TRANSLATE_HTML7(&me->textarea.data,
				me->UCLYhndl,
				current_char_set,
				NO,
				me->UsePlainSpace, me->HiddenValue,
				NO);
	    }
	    data = me->textarea.data;

	     
	    if (*data != '\0') {
		cp = (data + strlen(data)) - 1;
		while (cp >= data && *cp == '\n') {
		    *cp-- = '\0';
		}
		while (*data == '\n') {
		    data++;
		}
	    }
	     
	    if ((cp = StrChr(data, '\n')) != NULL) {
		*cp = '\0';
		StrAllocCopy(temp, data);
		*cp = '\n';
		data = (cp + 1);
	    } else {
		if (*data != '\0') {
		    StrAllocCopy(temp, data);
		} else {
		    FREE(temp);
		}
		data = empty;
	    }
	     
	    for (i = 0; i < me->textarea_rows; i++) {
		int j;

		for (j = 0; temp && temp[j]; j++) {
		    if (temp[j] == '\r')
			temp[j] = (char) (temp[j + 1] ? ' ' : '\0');
		}
		I.value = temp;
		chars = HText_beginInput(me->text, me->inUnderline, &I);
		for (; chars > 0; chars--)
		    HTML_put_character(me, '_');
		HText_appendCharacter(me->text, '\r');
		if (*data != '\0') {
		    if (*data == '\n') {
			FREE(temp);
			data++;
		    } else if ((cp = StrChr(data, '\n')) != NULL) {
			*cp = '\0';
			StrAllocCopy(temp, data);
			*cp = '\n';
			data = (cp + 1);
		    } else {
			StrAllocCopy(temp, data);
			data = empty;
		    }
		} else {
		    FREE(temp);
		}
	    }
	     
	    while (*data != '\0' || temp != NULL) {
		int j;

		for (j = 0; temp && temp[j]; j++) {
		    if (temp[j] == '\r')
			temp[j] = (char) (temp[j + 1] ? ' ' : '\0');
		}
		I.value = temp;
		(void) HText_beginInput(me->text, me->inUnderline, &I);
		for (chars = me->textarea_cols; chars > 0; chars--)
		    HTML_put_character(me, '_');
		HText_appendCharacter(me->text, '\r');
		if (*data == '\n') {
		    FREE(temp);
		    data++;
		} else if ((cp = StrChr(data, '\n')) != NULL) {
		    *cp = '\0';
		    StrAllocCopy(temp, data);
		    *cp = '\n';
		    data = (cp + 1);
		} else if (*data != '\0') {
		    StrAllocCopy(temp, data);
		    data = empty;
		} else {
		    FREE(temp);
		}
	    }
	    FREE(temp);
	    cp = NULL;
	    me->UsePlainSpace = FALSE;

	    HTChunkClear(&me->textarea);
	    FREE(me->textarea_name);
	    me->textarea_name_cs = -1;
	    FREE(me->textarea_id);
	    break;
	}

    case HTML_SELECT:
	{
	    char *ptr = NULL;

	     
	    if (!me->inSELECT) {
		if (LYBadHTML(me)) {
		    LYShowBadHTML("Bad HTML: Unmatched SELECT end tag *****\n");
		}
		break;
	    }

	     
	    me->inSELECT = FALSE;

	     
	    me->select_disabled = FALSE;

	     
	    if (!me->inFORM) {
		if (LYBadHTML(me)) {
		    LYShowBadHTML("Bad HTML: SELECT end tag not within FORM element *****\n");
		}
		 
	    }

	     
	    HTChunkTerminate(&me->option);
	     
	    if (!me->first_option)
		ptr = HText_setLastOptionValue(me->text,
					       me->option.data,
					       me->LastOptionValue,
					       LAST_ORDER,
					       me->LastOptionChecked,
					       me->UCLYhndl,
					       ATTR_CS_IN);
	    FREE(me->LastOptionValue);

	    me->LastOptionChecked = FALSE;

	    if (HTCurSelectGroupType == F_CHECKBOX_TYPE ||
		LYSelectPopups == FALSE) {
		 
		LYEnsureSingleSpace(me);
	    } else {
		 
		if (ptr &&
		    (me->sp[0].tag_number == HTML_PRE || me->inPRE == TRUE ||
		     !me->sp->style->freeFormat) &&
		    strlen(ptr) > 6) {
		     
		    for (i = 0; i < 6; i++) {
			if (*ptr == ' ')
			    HText_appendCharacter(me->text, HT_NON_BREAK_SPACE);
			else
			    HText_appendCharacter(me->text, *ptr);
			ptr++;
		    }
		}
		for (; non_empty(ptr); ptr++) {
		    if (*ptr == ' ')
			HText_appendCharacter(me->text, HT_NON_BREAK_SPACE);
		    else {
			HTkcode kcode = NOKANJI;
			HTkcode specified_kcode = NOKANJI;

			if (HTCJK == JAPANESE) {
			    kcode = HText_getKcode(me->text);
			    HText_updateKcode(me->text, kanji_code);
			    specified_kcode = HText_getSpecifiedKcode(me->text);
			    HText_updateSpecifiedKcode(me->text, kanji_code);
			}
			HText_appendCharacter(me->text, *ptr);
			if (HTCJK == JAPANESE) {
			    HText_updateKcode(me->text, kcode);
			    HText_updateSpecifiedKcode(me->text, specified_kcode);
			}
		    }
		}
		 
		if (!me->first_option) {
		    HText_appendCharacter(me->text, ']');
		    HText_endInput(me->text);
		    HText_setLastChar(me->text, ']');
		    me->in_word = YES;
		}
	    }
	    HTChunkClear(&me->option);

	    if (me->Underline_Level > 0 && me->inUnderline == FALSE) {
		HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
		me->inUnderline = TRUE;
	    }
	    if (me->needBoldH == TRUE && me->inBoldH == FALSE) {
		HText_appendCharacter(me->text, LY_BOLD_START_CHAR);
		me->inBoldH = TRUE;
		me->needBoldH = FALSE;
	    }
	}
	break;

    case HTML_TABLE:
#ifdef EXP_NESTED_TABLES
	if (!nested_tables)
#endif
	    me->inTABLE = FALSE;

	if (me->sp->style->id == ST_Preformatted) {
	    break;
	}
	if (me->Division_Level >= 0)
	    me->Division_Level--;
	if (me->Division_Level >= 0)
	    me->sp->style->alignment =
		me->DivisionAlignments[me->Division_Level];
	change_paragraph_style(me, me->sp->style);
	UPDATE_STYLE;

#ifdef EXP_NESTED_TABLES
	if (nested_tables) {
	    me->inTABLE = HText_endStblTABLE(me->text);
	} else {
	    HText_endStblTABLE(me->text);
	}
#else
	HText_endStblTABLE(me->text);
#endif

	me->current_default_alignment = me->sp->style->alignment;
	if (me->List_Nesting_Level >= 0)
	    HText_NegateLineOne(me->text);
	break;

 
    case HTML_TR:
	HText_endStblTR(me->text);
	if (!HText_LastLineEmpty(me->text, FALSE)) {
	    HText_setLastChar(me->text, ' ');	 
	    HText_appendCharacter(me->text, '\r');
	}
	me->in_word = NO;
	break;

    case HTML_THEAD:
    case HTML_TFOOT:
    case HTML_TBODY:
	break;

    case HTML_COLGROUP:
	if (me->inTABLE)
	    HText_endStblCOLGROUP(me->text);
	break;

    case HTML_TH:
    case HTML_TD:
	HText_endStblTD(me->text);
	break;

 
    case HTML_DT:
    case HTML_DD:
    case HTML_LH:
    case HTML_LI:
    case HTML_OVERLAY:
	break;

    case HTML_MATH:
	 
	HTChunkPutc(&me->math, ' ');
	HTChunkTerminate(&me->math);
	if (me->math.size > 2) {
	    LYEnsureSingleSpace(me);
	    if (me->inUnderline == FALSE)
		HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	    HTML_put_string(me, "[MATH:");
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	    HTML_put_character(me, ' ');
	    HTML_put_string(me, me->math.data);
	    HText_appendCharacter(me->text, LY_UNDERLINE_START_CHAR);
	    HTML_put_string(me, ":MATH]");
	    if (me->inUnderline == FALSE)
		HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	    LYEnsureSingleSpace(me);
	}
	HTChunkClear(&me->math);
	break;

    default:
	change_paragraph_style(me, me->sp->style);	 
	break;

    }				 

#ifdef USE_JUSTIFY_ELTS
    if (reached_awaited_stacked_elt)
	wait_for_this_stacked_elt = -1;
#endif

    if (me->xinclude) {
	HText_appendText(me->text, " *** LYNX ERROR ***\rUnparsed data:\r");
	HText_appendText(me->text, me->xinclude);
	FREE(me->xinclude);
    }
#ifdef USE_COLOR_STYLE
    if (!skip_stack_requested) {	 
# if !OMIT_SCN_KEEPING
	FastTrimColorClass(HTML_dtd.tags[element_number].name,
			   HTML_dtd.tags[element_number].name_len,
			   Style_className,
			   &Style_className_end, &hcode);
#  endif

	if (!ReallyEmptyTagNum(element_number)) {
	    CTRACE2(TRACE_STYLE,
		    (tfp,
		     "STYLE.end_element: ending non-\"EMPTY\" style <%s...>\n",
		     HTML_dtd.tags[element_number].name));
	    HText_characterStyle(me->text, HCODE_TO_STACK_OFF(hcode), STACK_OFF);
	}
    }
#endif  
    return status;
}
