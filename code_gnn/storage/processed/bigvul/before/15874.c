_handle_esc_csi_dsr(Termpty *ty, Eina_Unicode *b)
{
   int arg, len;
   char bf[32];

   if (*b == '>')
     {
        ERR("TODO: disable key resources used by xterm");
        return;
     }
   if (*b == '?')
     {
        b++;
        arg = _csi_arg_get(&b);
        switch (arg)
          {
           case 6:
              len = snprintf(bf, sizeof(bf), "\033[?%d;%d;1R",
                             ty->cursor_state.cy + 1, ty->cursor_state.cx + 1);
              termpty_write(ty, bf, len);
              break;
           default:
              WRN("unhandled DSR (dec specific) %d", arg);
              break;
          }
     }
   else
     {
        arg = _csi_arg_get(&b);
        switch (arg)
          {
           case 6:
              len = snprintf(bf, sizeof(bf), "\033[%d;%dR",
                             ty->cursor_state.cy + 1, ty->cursor_state.cx + 1);
              termpty_write(ty, bf, len);
              break;
           default:
              WRN("unhandled DSR %d", arg);
              break;
          }
     }
}
