static void HTML_free(HTStructured * me)
{
    char *include = NULL;

    if (LYMapsOnly && !me->text) {
	 
	FREE(me->base_href);
	FREE(me->map_address);
	clear_objectdata(me);
	FREE(me->xinclude);
	FREE(me);
	return;
    }

    UPDATE_STYLE;		 
    if (me->comment_end)
	HTML_put_string(me, me->comment_end);
    if (me->text) {
	 
	if (me->inUnderline) {
	    HText_appendCharacter(me->text, LY_UNDERLINE_END_CHAR);
	    me->inUnderline = FALSE;
	    me->Underline_Level = 0;
	    CTRACE((tfp, "HTML_free: Ending underline\n"));
	}
	if (me->inA) {
	    HTML_end_element(me, HTML_A, &include);
	    me->inA = FALSE;
	    CTRACE((tfp, "HTML_free: Ending HTML_A\n"));
	}
	if (me->inFONT) {
	    HTML_end_element(me, HTML_FONT, &include);
	    me->inFONT = FALSE;
	}
	if (me->inFORM) {
	    HTML_end_element(me, HTML_FORM, &include);
	    me->inFORM = FALSE;
	}
	if (me->option.size > 0) {
	     
	    if (LYBadHTML(me)) {
		LYShowBadHTML("Bad HTML: SELECT or OPTION not ended properly *****\n");
	    }
	    HTChunkTerminate(&me->option);
	     
	    CTRACE((tfp, "HTML_free: ***** leftover option data: %s\n",
		    me->option.data));
	    HTML_put_string(me, me->option.data);
	    HTChunkClear(&me->option);
	}
	if (me->textarea.size > 0) {
	     
	    if (LYBadHTML(me)) {
		LYShowBadHTML("Bad HTML: TEXTAREA not used properly *****\n");
	    }
	    HTChunkTerminate(&me->textarea);
	     
	    CTRACE((tfp, "HTML_free: ***** leftover textarea data: %s\n",
		    me->textarea.data));
	    HTML_put_string(me, me->textarea.data);
	    HTChunkClear(&me->textarea);
	}
	 
	if (!dump_output_immediately &&
	    HText_sourceAnchors(me->text) < 1 &&
	    HText_HiddenLinkCount(me->text) > 0) {
	    HTML_start_element(me, HTML_P, 0, 0, -1, &include);
	    HTML_put_character(me, '[');
	    HTML_start_element(me, HTML_EM, 0, 0, -1, &include);
	    HTML_put_string(me,
			    gettext("Document has only hidden links.  Use the 'l'ist command."));
	    HTML_end_element(me, HTML_EM, &include);
	    HTML_put_character(me, ']');
	    HTML_end_element(me, HTML_P, &include);
	}
	if (me->xinclude) {
	    HText_appendText(me->text, " *** LYNX ERROR ***\rUnparsed data:\r");
	    HText_appendText(me->text, me->xinclude);
	    FREE(me->xinclude);
	}

	 
	HText_endAppend(me->text);
    }
    if (me->option.size > 0) {
	 
	if (LYBadHTML(me)) {
	    LYShowBadHTML("Bad HTML: SELECT or OPTION not ended properly *****\n");
	}
	if (TRACE) {
	    HTChunkTerminate(&me->option);
	    CTRACE((tfp, "HTML_free: ***** leftover option data: %s\n",
		    me->option.data));
	}
	HTChunkClear(&me->option);
    }
    if (me->textarea.size > 0) {
	 
	if (LYBadHTML(me)) {
	    LYShowBadHTML("Bad HTML: TEXTAREA not used properly *****\n");
	}
	if (TRACE) {
	    HTChunkTerminate(&me->textarea);
	    CTRACE((tfp, "HTML_free: ***** leftover textarea data: %s\n",
		    me->textarea.data));
	}
	HTChunkClear(&me->textarea);
    }

    if (me->target) {
	(*me->targetClass._free) (me->target);
    }
    if (me->sp && me->sp->style && GetHTStyleName(me->sp->style)) {
	if (me->sp->style->id == ST_DivCenter ||
	    me->sp->style->id == ST_HeadingCenter ||
	    me->sp->style->id == ST_Heading1) {
	    me->sp->style->alignment = HT_CENTER;
	} else if (me->sp->style->id == ST_DivRight ||
		   me->sp->style->id == ST_HeadingRight) {
	    me->sp->style->alignment = HT_RIGHT;
	} else {
	    me->sp->style->alignment = HT_LEFT;
	}
	styles[HTML_PRE]->alignment = HT_LEFT;
    }
    FREE(me->base_href);
    FREE(me->map_address);
    FREE(me->LastOptionValue);
    clear_objectdata(me);
    FREE(me);
}
