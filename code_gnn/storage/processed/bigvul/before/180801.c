  void HTML_put_string(HTStructured * me, const char *s)
  {
//     HTChunk *target = NULL;
// 
  #ifdef USE_PRETTYSRC
      char *translated_string = NULL;
  #endif
 
     if (s == NULL || (LYMapsOnly && me->sp[0].tag_number != HTML_OBJECT))
 	return;
 #ifdef USE_PRETTYSRC
     if (psrc_convert_string) {
 	StrAllocCopy(translated_string, s);
 	TRANSLATE_AND_UNESCAPE_ENTITIES(&translated_string, TRUE, FALSE);
 	s = (const char *) translated_string;
     }
 #endif
 
     switch (me->sp[0].tag_number) {
 
     case HTML_COMMENT:
  	break;			 
  
      case HTML_TITLE:
	HTChunkPuts(&me->title, s);
// 	target = &me->title;
  	break;
  
      case HTML_STYLE:
	HTChunkPuts(&me->style_block, s);
// 	target = &me->style_block;
  	break;
  
      case HTML_SCRIPT:
	HTChunkPuts(&me->script, s);
// 	target = &me->script;
  	break;
  
      case HTML_PRE:		 
     case HTML_LISTING:		 
     case HTML_XMP:
     case HTML_PLAINTEXT:
 	 
 	HText_appendText(me->text, s);
  	break;
  
      case HTML_OBJECT:
	HTChunkPuts(&me->object, s);
// 	target = &me->object;
  	break;
  
      case HTML_TEXTAREA:
	HTChunkPuts(&me->textarea, s);
// 	target = &me->textarea;
  	break;
  
      case HTML_SELECT:
      case HTML_OPTION:
	HTChunkPuts(&me->option, s);
// 	target = &me->option;
  	break;
  
      case HTML_MATH:
	HTChunkPuts(&me->math, s);
// 	target = &me->math;
  	break;
  
      default:			 
 	if (!me->sp->style->freeFormat) {
 	     
 #ifdef USE_PRETTYSRC
 	    if (psrc_view) {
 		 
 		for (; *s; ++s)
 		    HTML_put_character(me, *s);
 	    } else
 #endif
 		HText_appendText(me->text, s);
 	    break;
 	} else {
 	    const char *p = s;
 	    char c;
 
 	    if (me->style_change) {
 		for (; *p && ((*p == '\n') || (*p == '\r') ||
 			      (*p == ' ') || (*p == '\t')); p++) ;	 
 		if (!*p)
 		    break;
 		UPDATE_STYLE;
 	    }
 	    for (; *p; p++) {
 		if (*p == 13 && p[1] != 10) {
 		     
 		    c = '\n';
 		} else {
 		    c = *p;
 		}
 		if (me->style_change) {
 		    if ((c == '\n') || (c == ' ') || (c == '\t'))
 			continue;	 
 		    UPDATE_STYLE;
 		}
 		if (c == '\n') {
 		    if (!FIX_JAPANESE_SPACES) {
 			if (me->in_word) {
 			    if (HText_getLastChar(me->text) != ' ')
 				HText_appendCharacter(me->text, ' ');
 			    me->in_word = NO;
 			}
 		    }
 
 		} else if (c == ' ' || c == '\t') {
 		    if (HText_getLastChar(me->text) != ' ')
 			HText_appendCharacter(me->text, ' ');
 
 		} else if (c == '\r') {
 		     
 		} else {
 		    HText_appendCharacter(me->text, c);
 		    me->in_word = YES;
 		}
 
 		 
 		if (c == '\n' || c == '\t') {
 		     
 		    HText_setLastChar(me->text, ' ');
 		} else if (c == '\r' &&
 			   HText_getLastChar(me->text) == ' ') {
 		     
 		    HText_setLastChar(me->text, ' ');
 		} else {
 		    HText_setLastChar(me->text, c);
 		}
 
  	    }			 
  	}
      }				 
// 
//     if (target != NULL) {
// 	if (target->data == s) {
// 	    CTRACE((tfp, "BUG: appending chunk to itself: `%.*s'\n",
// 		    target->size, target->data));
// 	} else {
// 	    HTChunkPuts(target, s);
// 	}
//     }
  #ifdef USE_PRETTYSRC
      if (psrc_convert_string) {
  	psrc_convert_string = FALSE;
 	FREE(translated_string);
     }
 #endif
 }