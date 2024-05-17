__imlib_RenderDisconnect(Display * d)
{
   if (d != disp)
      return;
   disp = NULL;
   gc = gcm = NULL;
   last_depth = 0;
}
