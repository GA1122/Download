_handle_esc_csi(Termpty *ty, const Eina_Unicode *c, Eina_Unicode *ce)
{
   int arg, i;
   const Eina_Unicode *cc, *be;
   Eina_Unicode buf[4096], *b;

   cc = (Eina_Unicode *)c;
   b = buf;
   be = buf + sizeof(buf) / sizeof(buf[0]);
   while ((cc < ce) && (*cc <= '?') && (b < be))
     {
        _handle_cursor_control(ty, cc);
        *b = *cc;
        b++;
        cc++;
     }
   if (b == be)
     {
        ERR("csi parsing overflowed, skipping the whole buffer (binary data?)");
        return cc - c;
     }
   if (cc == ce) return 0;
   *b = 0;
   b = buf;
   DBG(" CSI: '%s' args '%s'", _safechar(*cc), (char *) buf);
   switch (*cc)
     {
      case 'm':  
        _handle_esc_csi_color_set(ty, &b);
        break;
      case '@':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("insert %d blank chars", arg);
          {
             int pi = ty->termstate.insert;
             Eina_Unicode blank[1] = { ' ' };
             int cx = ty->cursor_state.cx;

             ty->termstate.wrapnext = 0;
             ty->termstate.insert = 1;
             for (i = 0; i < arg; i++)
               termpty_text_append(ty, blank, 1);
             ty->termstate.insert = pi;
             ty->cursor_state.cx = cx;
             TERMPTY_RESTRICT_FIELD(ty->cursor_state.cx, 0, ty->w);
          }
        break;
      case 'A':  
      case 'e':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("cursor up %d", arg);
        ty->termstate.wrapnext = 0;
        ty->cursor_state.cy = MAX(0, ty->cursor_state.cy - arg);
        TERMPTY_RESTRICT_FIELD(ty->cursor_state.cy, 0, ty->h);
        break;
      case 'B':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("cursor down %d", arg);
        ty->termstate.wrapnext = 0;
        ty->cursor_state.cy = MIN(ty->h - 1, ty->cursor_state.cy + arg);
        TERMPTY_RESTRICT_FIELD(ty->cursor_state.cy, 0, ty->h);
        break;
      case 'D':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("cursor left %d", arg);
        ty->termstate.wrapnext = 0;
        for (i = 0; i < arg; i++)
          ty->cursor_state.cx--;
        TERMPTY_RESTRICT_FIELD(ty->cursor_state.cx, 0, ty->w);
        break;
      case 'C':  
      case 'a':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("cursor right %d", arg);
        ty->termstate.wrapnext = 0;
        for (i = 0; i < arg; i++)
          ty->cursor_state.cx++;
        TERMPTY_RESTRICT_FIELD(ty->cursor_state.cx, 0, ty->w);
        break;
      case 'H':  
      case 'f':  
        DBG("cursor pos set");
        ty->termstate.wrapnext = 0;
        if (!*b)
          {
             ty->cursor_state.cx = 0;
             ty->cursor_state.cy = 0;
          }
        else
          {
             arg = _csi_arg_get(&b);
             if (arg < 1) arg = 1;
             arg--;
             if (arg >= ty->h) arg = ty->h - 1;
             if (b)
               {
                  ty->cursor_state.cy = arg;
                  TERMPTY_RESTRICT_FIELD(ty->cursor_state.cy, 0, ty->h);
                  arg = _csi_arg_get(&b);
                  if (arg < 1) arg = 1;
                  arg--;
               }
             else arg = 0;

             if (arg >= ty->w) arg = ty->w - 1;
             if (b)
               {
                  ty->cursor_state.cx = arg;
                  TERMPTY_RESTRICT_FIELD(ty->cursor_state.cx, 0, ty->w);
               }
          }
        ty->cursor_state.cy += ty->termstate.margin_top;
        TERMPTY_RESTRICT_FIELD(ty->cursor_state.cy, 0, ty->h);
       break;
      case 'G':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("to column %d", arg);
        ty->termstate.wrapnext = 0;
        ty->cursor_state.cx = arg - 1;
        TERMPTY_RESTRICT_FIELD(ty->cursor_state.cx, 0, ty->w);
        break;
      case 'd':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("to row %d", arg);
        ty->termstate.wrapnext = 0;
        ty->cursor_state.cy = arg - 1;
        TERMPTY_RESTRICT_FIELD(ty->cursor_state.cy, 0, ty->h);
        break;
      case 'E':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("down relative %d rows, and to col 0", arg);
        ty->termstate.wrapnext = 0;
        ty->cursor_state.cy += arg;
        TERMPTY_RESTRICT_FIELD(ty->cursor_state.cy, 0, ty->h);
        ty->cursor_state.cx = 0;
        break;
      case 'F':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("up relative %d rows, and to col 0", arg);
        ty->termstate.wrapnext = 0;
        ty->cursor_state.cy -= arg;
        TERMPTY_RESTRICT_FIELD(ty->cursor_state.cy, 0, ty->h);
        ty->cursor_state.cx = 0;
        break;
      case 'X':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("erase %d chars", arg);
        termpty_clear_line(ty, TERMPTY_CLR_END, arg);
        break;
      case 'S':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("scroll up %d lines", arg);
        for (i = 0; i < arg; i++)
          termpty_text_scroll(ty, EINA_TRUE);
        break;
      case 'T':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("scroll down %d lines", arg);
        for (i = 0; i < arg; i++)
          termpty_text_scroll_rev(ty, EINA_TRUE);
        break;
      case 'M':  
      case 'L':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("%s %d lines", (*cc == 'M') ? "delete" : "insert", arg);
          {
             int sy1, sy2;

             sy1 = ty->termstate.scroll_y1;
             sy2 = ty->termstate.scroll_y2;
             if (ty->termstate.scroll_y2 == 0)
               {
                  ty->termstate.scroll_y1 = ty->cursor_state.cy;
                  ty->termstate.scroll_y2 = ty->h;
               }
             else
               {
                  ty->termstate.scroll_y1 = ty->cursor_state.cy;
                  if (ty->termstate.scroll_y2 <= ty->termstate.scroll_y1)
                    ty->termstate.scroll_y2 = ty->termstate.scroll_y1 + 1;
               }
             for (i = 0; i < arg; i++)
               {
                  if (*cc == 'M')
                    termpty_text_scroll(ty, EINA_TRUE);
                  else
                    termpty_text_scroll_rev(ty, EINA_TRUE);
               }
             ty->termstate.scroll_y1 = sy1;
             ty->termstate.scroll_y2 = sy2;
          }
        break;
      case 'P':  
        arg = _csi_arg_get(&b);
        if (arg < 1) arg = 1;
        DBG("erase and scrollback %d chars", arg);
          {
             Termcell *cells;
             int x, lim;

             cells = &(TERMPTY_SCREEN(ty, 0, ty->cursor_state.cy));
             lim = ty->w - arg;
             for (x = ty->cursor_state.cx; x < (ty->w); x++)
               {
                  if (x < lim)
                    termpty_cell_copy(ty, &(cells[x + arg]), &(cells[x]), 1);
                  else
                    {
                       cells[x].codepoint = ' ';
                       cells[x].att.underline = 0;
                       cells[x].att.blink = 0;
                       cells[x].att.blink2 = 0;
                       cells[x].att.inverse = 0;
                       cells[x].att.strike = 0;
#if defined(SUPPORT_DBLWIDTH)
                       cells[x].att.dblwidth = 0;
#endif
                    }
               }
          }
        break;
      case 'c':  
        DBG("query device attributes");
          {
             char bf[32];
             if (b && *b == '>')
               {
                  snprintf(bf, sizeof(bf), "\033[>41;285;%ic", 0);
               }
             else
               {
                  snprintf(bf, sizeof(bf), "\033[?64;1;2;6;9;15;18;21;22c");
               }
             termpty_write(ty, bf, strlen(bf));
          }
        break;
      case 'J':  
        DBG("2j erases the screen, 1j erase from screen start to curs, 0j erase cursor to end of screen");
        arg = _csi_arg_get(&b);
        if (b)
          {
             if ((arg >= TERMPTY_CLR_END) && (arg <= TERMPTY_CLR_ALL))
               termpty_clear_screen(ty, arg);
             else
               ERR("invalid clr scr %i", arg);
          }
        else
          termpty_clear_screen(ty, TERMPTY_CLR_END);
        break;
      case 'K':  
        arg = _csi_arg_get(&b);
        DBG("0K erase to end of line, 1K erase from screen start to cursor, 2K erase all of line: %d", arg);
        if (b)
          {
             if ((arg >= TERMPTY_CLR_END) && (arg <= TERMPTY_CLR_ALL))
               termpty_clear_line(ty, arg, ty->w);
             else
               ERR("invalid clr lin %i", arg);
          }
        else termpty_clear_line(ty, TERMPTY_CLR_END, ty->w);
        break;
      case 'h':
      case 'l':
        _handle_esc_csi_reset_mode(ty, *cc, b);
        break;
      case 'r':
        arg = _csi_arg_get(&b);
        if (!b)
          {
             WRN("no region args reset region");
             ty->termstate.scroll_y1 = 0;
             ty->termstate.scroll_y2 = 0;
          }
        else
          {
             int arg2;

             arg2 = _csi_arg_get(&b);
             if (!b)
               {
                  WRN("failed to give 2 regions args reset region");
                  ty->termstate.scroll_y1 = 0;
                  ty->termstate.scroll_y2 = 0;
               }
             else
               {
                  if (arg > arg2)
                    {
                       DBG("scroll region beginning > end [%i %i]", arg, arg2);
                       ty->termstate.scroll_y1 = 0;
                       ty->termstate.scroll_y2 = 0;
                    }
                  else
                    {
                       DBG("2 regions args: %i %i", arg, arg2);
                       if (arg >= ty->h) arg = ty->h - 1;
                       if (arg == 0) arg = 1;
                       if (arg2 > ty->h) arg2 = ty->h;
                       ty->termstate.scroll_y1 = arg - 1;
                       ty->termstate.scroll_y2 = arg2;
                       if ((arg == 1) && (arg2 == ty->h))
                          ty->termstate.scroll_y2 = 0;
                    }
               }
          }
        break;
      case 's':  
        termpty_cursor_copy(ty, EINA_TRUE);
        break;
      case 'u':  
        termpty_cursor_copy(ty, EINA_FALSE);
        break;
      case 'p':  
        if (b && *b == '!')
          {
             DBG("soft reset (DECSTR)");
             termpty_reset_state(ty);
          }
        else
          {
             goto unhandled;
          }
        break;
      case 'n':
        _handle_esc_csi_dsr(ty, b);
        break;
 
      case 'g':  
        arg = _csi_arg_get(&b);
        DBG("Tabulation Clear (TBC): %d", arg);
        break;
       case 'Z':  
       {
          int idx, size, cx = ty->cursor_state.cx, cy = ty->cursor_state.cy;

          arg = _csi_arg_get(&b);
          if (arg < 1) arg = 1;

          size = ty->w * cy + cx + 1;
          for (idx = size - 1; idx >= 0; idx--)
            {
               if (TERMPTY_SCREEN(ty, cx, cy).att.tab) arg--;
               cx--;
               if (cx < 0)
                 {
                    cx = ty->w - 1;
                    cy--;
                 }
               if (!arg) break;
            }
          if (!arg)
            {
               ty->cursor_state.cx = cx;
               TERMPTY_RESTRICT_FIELD(ty->cursor_state.cx, 0, ty->w);
               ty->cursor_state.cy = cy;
               TERMPTY_RESTRICT_FIELD(ty->cursor_state.cy, 0, ty->h);
            }
       }
       break;
      default:
       goto unhandled;
     }
   cc++;
   return cc - c;
unhandled:
     {
        Eina_Strbuf *bf = eina_strbuf_new();

        for (i = 0; c + i <= cc && i < 100; i++)
          {
             if ((c[i] < ' ') || (c[i] >= 0x7f))
               eina_strbuf_append_printf(bf, "\033[35m%08x\033[0m",
                                         (unsigned int) c[i]);
             else
               eina_strbuf_append_char(bf, c[i]);
          }
        ERR("unhandled CSI '%s': %s", _safechar(*cc), eina_strbuf_string_get(bf));
        eina_strbuf_free(bf);
     }
   cc++;
   return cc - c;
}
