PNG_FUNCTION(void, display_exit, (display *d), static PNG_NORETURN)
{
 ++(d->error_count);

 if (d->png_ptr != NULL)
      clean_display(d);

  
 if (d->test == init || d->test == cmd)
      exit(1);

   longjmp(d->error_return, 1);
}
