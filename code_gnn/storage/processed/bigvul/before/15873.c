_handle_esc_csi_color_set(Termpty *ty, Eina_Unicode **ptr)
{
   Eina_Unicode *b = *ptr;
   int first = 1;

   if (b && (*b == '>'))
     {  
        ERR("TODO: set/reset key resources used by xterm");
        return;
     }
   DBG("color set");
   while (b)
     {
        int arg = _csi_arg_get(&b);
        if ((first) && (!b))
          termpty_reset_att(&(ty->termstate.att));
        else if (b)
          {
             first = 0;
             switch (arg)
               {
                case 0:  
                   termpty_reset_att(&(ty->termstate.att));
                   break;
                case 1:  
                   ty->termstate.att.bold = 1;
                   break;
                case 2:  
                   ty->termstate.att.faint = 1;
                   break;
                case 3:  
#if defined(SUPPORT_ITALIC)
                   ty->termstate.att.italic = 1;
#endif
                   break;
                case 4:  
                   ty->termstate.att.underline = 1;
                   break;
                case 5:  
                   ty->termstate.att.blink = 1;
                   break;
                case 6:  
                   ty->termstate.att.blink2 = 1;
                   break;
                case 7:  
                   ty->termstate.att.inverse = 1;
                   break;
                case 8:  
                   ty->termstate.att.invisible = 1;
                   break;
                case 9:  
                   ty->termstate.att.strike = 1;
                   break;
                case 20:  
                   ty->termstate.att.fraktur = 1;
                   break;
                case 21:  
                   ty->termstate.att.bold = 0;
                   break;
                case 22:  
                   ty->termstate.att.bold = 0;
                   ty->termstate.att.faint = 0;
                   break;
                case 23:  
#if defined(SUPPORT_ITALIC)
                   ty->termstate.att.italic = 0;
#endif
                   ty->termstate.att.fraktur = 0;
                   break;
                case 24:  
                   ty->termstate.att.underline = 0;
                   break;
                case 25:  
                   ty->termstate.att.blink = 0;
                   ty->termstate.att.blink2 = 0;
                   break;
                case 27:  
                   ty->termstate.att.inverse = 0;
                   break;
                case 28:  
                   ty->termstate.att.invisible = 0;
                   break;
                case 29:  
                   ty->termstate.att.strike = 0;
                   break;
                case 30:  
                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                   ty->termstate.att.fg256 = 0;
                   ty->termstate.att.fg = (arg - 30) + COL_BLACK;
                   ty->termstate.att.fgintense = 0;
                   break;
                case 38:  
                   arg = _csi_arg_get(&b);
                   if (arg != 5) ERR("Failed xterm 256 color fg esc 5 (got %d)", arg);
                   else
                     {
                        arg = _csi_arg_get(&b);
                        if (!b) ERR("Failed xterm 256 color fg esc val");
                        else
                          {
                             ty->termstate.att.fg256 = 1;
                             ty->termstate.att.fg = arg;
                          }
                     }
                   ty->termstate.att.fgintense = 0;
                   break;
                case 39:  
                   ty->termstate.att.fg256 = 0;
                   ty->termstate.att.fg = COL_DEF;
                   ty->termstate.att.fgintense = 0;
                   break;
                case 40:  
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                   ty->termstate.att.bg256 = 0;
                   ty->termstate.att.bg = (arg - 40) + COL_BLACK;
                   ty->termstate.att.bgintense = 0;
                   break;
                case 48:  
                   arg = _csi_arg_get(&b);
                   if (arg != 5) ERR("Failed xterm 256 color bg esc 5 (got %d)", arg);
                   else
                     {
                        arg = _csi_arg_get(&b);
                        if (!b) ERR("Failed xterm 256 color bg esc val");
                        else
                          {
                             ty->termstate.att.bg256 = 1;
                             ty->termstate.att.bg = arg;
                          }
                     }
                   ty->termstate.att.bgintense = 0;
                   break;
                case 49:  
                   ty->termstate.att.bg256 = 0;
                   ty->termstate.att.bg = COL_DEF;
                   ty->termstate.att.bgintense = 0;
                   break;
                case 90:  
                case 91:
                case 92:
                case 93:
                case 94:
                case 95:
                case 96:
                case 97:
                   ty->termstate.att.fg256 = 0;
                   ty->termstate.att.fg = (arg - 90) + COL_BLACK;
                   ty->termstate.att.fgintense = 1;
                   break;
                case 98:  
                   arg = _csi_arg_get(&b);
                   if (arg != 5) ERR("Failed xterm 256 color fg esc 5 (got %d)", arg);
                   else
                     {
                        arg = _csi_arg_get(&b);
                        if (!b) ERR("Failed xterm 256 color fg esc val");
                        else
                          {
                             ty->termstate.att.fg256 = 1;
                             ty->termstate.att.fg = arg;
                          }
                     }
                   ty->termstate.att.fgintense = 1;
                   break;
                case 99:  
                   ty->termstate.att.fg256 = 0;
                   ty->termstate.att.fg = COL_DEF;
                   ty->termstate.att.fgintense = 1;
                   break;
                case 100:  
                case 101:
                case 102:
                case 103:
                case 104:
                case 105:
                case 106:
                case 107:
                   ty->termstate.att.bg256 = 0;
                   ty->termstate.att.bg = (arg - 100) + COL_BLACK;
                   ty->termstate.att.bgintense = 1;
                   break;
                case 108:  
                   arg = _csi_arg_get(&b);
                   if (arg != 5) ERR("Failed xterm 256 color bg esc 5 (got %d)", arg);
                   else
                     {
                        arg = _csi_arg_get(&b);
                        if (!b) ERR("Failed xterm 256 color bg esc val");
                        else
                          {
                             ty->termstate.att.bg256 = 1;
                             ty->termstate.att.bg = arg;
                          }
                     }
                   ty->termstate.att.bgintense = 1;
                   break;
                case 109:  
                   ty->termstate.att.bg256 = 0;
                   ty->termstate.att.bg = COL_DEF;
                   ty->termstate.att.bgintense = 1;
                   break;
                default:  
                   ERR("Unhandled color cmd [%i]", arg);
                   break;
               }
          }
     }
}
