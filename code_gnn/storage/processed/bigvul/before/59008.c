void HTML_put_character(HTStructured * me, int c)
{
    unsigned uc = UCH(c);

     
    if (LYMapsOnly && me->sp[0].tag_number != HTML_OBJECT)
	return;

    c = (int) uc;

     
    if ((me->lastraw == '\r') && c == '\n') {
	me->lastraw = -1;
	return;
    }
    me->lastraw = c;
    if (c == '\r') {
	c = '\n';
	uc = UCH(c);
    }

     
    switch (me->sp[0].tag_number) {

    case HTML_COMMENT:
	return;			 

    case HTML_TITLE:
	if (c == LY_SOFT_HYPHEN)
	    return;
	if (c != '\n' && c != '\t' && c != '\r') {
	    HTChunkPutc(&me->title, uc);
	} else if (FIX_JAPANESE_SPACES) {
	    if (c == '\t') {
		HTChunkPutc(&me->title, ' ');
	    } else {
		return;
	    }
	} else {
	    HTChunkPutc(&me->title, ' ');
	}
	return;

    case HTML_STYLE:
	HTChunkPutc(&me->style_block, uc);
	return;

    case HTML_SCRIPT:
	HTChunkPutc(&me->script, uc);
	return;

    case HTML_OBJECT:
	HTChunkPutc(&me->object, uc);
	return;

    case HTML_TEXTAREA:
	HTChunkPutc(&me->textarea, uc);
	return;

    case HTML_SELECT:
    case HTML_OPTION:
	HTChunkPutc(&me->option, uc);
	return;

    case HTML_MATH:
	HTChunkPutc(&me->math, uc);
	return;

    default:
	if (me->inSELECT) {
	     
	    if (me->sp[0].tag_number == HTML_A)
		break;
	    HTChunkPutc(&me->option, uc);
	    return;
	}
	break;
    }				 

     
    switch (me->sp[0].tag_number) {

    case HTML_PRE:		 
	 
	if (c != '\r' &&
	    !(c == '\n' && me->inLABEL && !me->inP) &&
	    !(c == '\n' && !me->inPRE)) {
	    me->inP = TRUE;
	    me->inLABEL = FALSE;
	    HText_appendCharacter(me->text, c);
	}
	me->inPRE = TRUE;
	break;

    case HTML_LISTING:		 
    case HTML_XMP:
    case HTML_PLAINTEXT:
	 
	if (c != '\r') {
	    me->inP = TRUE;
	    me->inLABEL = FALSE;
	    HText_appendCharacter(me->text, c);
	}
	break;

    default:
	 
	if (me->sp->style->id == ST_Preformatted) {
	    if (c != '\r' &&
		!(c == '\n' && me->inLABEL && !me->inP) &&
		!(c == '\n' && !me->inPRE)) {
		me->inP = TRUE;
		me->inLABEL = FALSE;
		HText_appendCharacter(me->text, c);
	    }
	    me->inPRE = TRUE;

	} else if (me->sp->style->id == ST_Listing ||
		   me->sp->style->id == ST_Example) {
	    if (c != '\r') {
		me->inP = TRUE;
		me->inLABEL = FALSE;
		HText_appendCharacter(me->text, c);
	    }

	} else {
	    if (me->style_change) {
		if ((c == '\n') || (c == ' '))
		    return;	 
		UPDATE_STYLE;
	    }
	    if (c == '\n') {
		if (!FIX_JAPANESE_SPACES) {
		    if (me->in_word) {
			if (HText_getLastChar(me->text) != ' ') {
			    me->inP = TRUE;
			    me->inLABEL = FALSE;
			    HText_appendCharacter(me->text, ' ');
			}
			me->in_word = NO;
		    }
		}

	    } else if (c == ' ' || c == '\t') {
		if (HText_getLastChar(me->text) != ' ') {
		    me->inP = TRUE;
		    me->inLABEL = FALSE;
		    HText_appendCharacter(me->text, ' ');
		}

	    } else if (c == '\r') {
		 

	    } else {
		me->inP = TRUE;
		me->inLABEL = FALSE;
		HText_appendCharacter(me->text, c);
		me->in_word = YES;
	    }
	}
    }				 

    if (c == '\n' || c == '\t') {
	HText_setLastChar(me->text, ' ');	 
    } else {
	HText_setLastChar(me->text, c);
    }
}
