_handle_esc_csi_reset_mode(Termpty *ty, Eina_Unicode cc, Eina_Unicode *b)
{
   int mode = 0, priv = 0, arg;

   if (cc == 'h') mode = 1;
   if (*b == '?')
     {
        priv = 1;
        b++;
     }
   if (priv)  
     {
        while (b)
          {
             arg = _csi_arg_get(&b);
             if (b)
               {
                  switch (arg)
                    {
                     case 1:
                        ty->termstate.appcursor = mode;
                        break;
                     case 2:
                        ty->termstate.kbd_lock = mode;
                        break;
                     case 3:  
#if defined(SUPPORT_80_132_COLUMNS)
                        if (ty->termstate.att.is_80_132_mode_allowed)
                          {
                              
                             Evas_Object *wn;
                             int w, h;

                             wn = termio_win_get(ty->obj);
                             elm_win_size_step_get(wn, &w, &h);
                             evas_object_resize(wn,
                                                4 +
                                                (mode ? 132 : 80) * w,
                                                4 + ty->h * h);
                             termpty_resize(ty, mode ? 132 : 80,
                                            ty->h);
                             termpty_reset_state(ty);
                             termpty_clear_screen(ty,
                                                   TERMPTY_CLR_ALL);
                          }
#endif
                        break;
                     case 4:
                        WRN("TODO: scrolling mode (DECSCLM): %i", mode);
                        break;
                     case 5:
                        ty->termstate.reverse = mode;
                        break;
                     case 6:
                        if (mode)
                          {
                             ty->termstate.margin_top = ty->cursor_state.cy;
                             ty->cursor_state.cx = 0;
                          }
                        else
                          {
                             ty->cursor_state.cx = 0;
                             ty->termstate.margin_top = 0;
                          }
                        DBG("origin mode (%d): cursor is at 0,0"
                            " cursor limited to screen/start point"
                            " for line #'s depends on top margin",
                            mode);
                        break;
                     case 7:
                        DBG("set wrap mode to %i", mode);
                        ty->termstate.wrap = mode;
                        break;
                     case 8:
                        ty->termstate.no_autorepeat = !mode;
                        DBG("auto repeat %i", mode);
                        break;
                     case 9:
                        DBG("set mouse (X10) %i", mode);
                        if (mode) ty->mouse_mode = MOUSE_X10;
                        else ty->mouse_mode = MOUSE_OFF;
                        break;
                     case 12:  
                        WRN("TODO: set blinking cursor to (stop?) %i or local echo (ignored)", mode);
                        break;
                     case 19:  
                        WRN("TODO: set print extent to full screen");
                        break;
                     case 20:  
                        ty->termstate.crlf = mode;
                        break;
                     case 25:
                        ty->termstate.hide_cursor = !mode;
                        DBG("hide cursor: %d", !mode);
                        break;
                     case 30:  
                        WRN("TODO: set scrollbar mapping %i", mode);
                        break;
                     case 33:  
                        WRN("TODO: Stop cursor blink %i", mode);
                        break;
                     case 34:  
                        WRN("TODO: Underline cursor mode %i", mode);
                        break;
                     case 35:  
                        WRN("TODO: set shift keys %i", mode);
                        break;
                     case 38:  
                        WRN("TODO: switch to tek window %i", mode);
                        break;
                     case 40:
                        DBG("Allow 80 -> 132 Mode %i", mode);
#if defined(SUPPORT_80_132_COLUMNS)
                        ty->termstate.att.is_80_132_mode_allowed = mode;
#endif
                        break;
                     case 45:  
                        WRN("TODO: Reverse-wraparound Mode");
                        break;
                     case 59:  
                        WRN("TODO: kanji terminal mode %i", mode);
                        break;
                     case 66:
                        WRN("TODO: app keypad mode %i", mode);
                        break;
                     case 67:
                        ty->termstate.send_bs = mode;
                        DBG("backspace send bs not del = %i", mode);
                        break;
                     case 1000:
                        if (mode) ty->mouse_mode = MOUSE_NORMAL;
                        else ty->mouse_mode = MOUSE_OFF;
                        DBG("set mouse (press+release only) to %i", mode);
                        break;
                     case 1001:
                        WRN("TODO: x11 mouse highlighting %i", mode);
                        break;
                     case 1002:
                        if (mode) ty->mouse_mode = MOUSE_NORMAL_BTN_MOVE;
                        else ty->mouse_mode = MOUSE_OFF;
                        DBG("set mouse (press+release+motion while pressed) %i", mode);
                        break;
                     case 1003:
                        if (mode) ty->mouse_mode = MOUSE_NORMAL_ALL_MOVE;
                        else ty->mouse_mode = MOUSE_OFF;
                        DBG("set mouse (press+release+all motion) %i", mode);
                        break;
                     case 1004:  
                        WRN("TODO: enable focus reporting %i", mode);
                        break;
                     case 1005:
                        if (mode) ty->mouse_ext = MOUSE_EXT_UTF8;
                        else ty->mouse_ext = MOUSE_EXT_NONE;
                        DBG("set mouse (xterm utf8 style) %i", mode);
                        break;
                     case 1006:
                        if (mode) ty->mouse_ext = MOUSE_EXT_SGR;
                        else ty->mouse_ext = MOUSE_EXT_NONE;
                        DBG("set mouse (xterm sgr style) %i", mode);
                        break;
                     case 1010:  
                        WRN("TODO: set home on tty output %i", mode);
                        break;
                     case 1012:  
                        WRN("TODO: set home on tty input %i", mode);
                        break;
                     case 1015:
                        if (mode) ty->mouse_ext = MOUSE_EXT_URXVT;
                        else ty->mouse_ext = MOUSE_EXT_NONE;
                        DBG("set mouse (rxvt-unicode style) %i", mode);
                        break;
                     case 1034:  
                         
                        DBG("Ignored screen mode %i", arg);
                        break;
                     case 1047:
                     case 47:
                        _switch_to_alternative_screen(ty, mode);
                        break;
                     case 1048:
                     case 1049:
                        termpty_cursor_copy(ty, mode);
                        if (arg == 1049)
                          _switch_to_alternative_screen(ty, mode);
                        break;
                     case 2004:
                        ty->bracketed_paste = mode;
                        break;
                     case 7727:  
                        WRN("TODO: enable application escape mode %i", mode);
                        break;
                     case 7786:  
                        WRN("TODO: enable mouse wheel -> cursor key xlation %i", mode);
                        break;
                     default:
                        ERR("Unhandled DEC Private Reset Mode arg %i", arg);
                        break;
                    }
               }
          }
     }
   else  
     {
        while (b)
          {
             arg = _csi_arg_get(&b);
             if (b)
               {
                  switch (arg)
                    {
                     case 1:
                        ty->termstate.appcursor = mode;
                        break;
                     case 4:
                        DBG("set insert mode to %i", mode);
                        ty->termstate.insert = mode;
                        break;
                     case 34:
                        WRN("TODO: hebrew keyboard mapping: %i", mode);
                        break;
                     case 36:
                        WRN("TODO: hebrew encoding mode: %i", mode);
                        break;
                     default:
                        ERR("Unhandled ANSI Reset Mode arg %i", arg);
                    }
               }
          }
     }
}
