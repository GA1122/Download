skip_transform(struct display *dp, int tr)
  
{
 if ((dp->options & SKIP_BUGS) != 0 && is_bad_combo(tr))
 {
  
 if ((dp->options & LOG_SKIPPED) != 0)
 {
         printf("SKIP: %s transforms ", dp->filename);

 while (tr != 0)
 {
 int next = first_transform(tr);
            tr &= ~next;

            printf("%s", transform_name(next));
 if (tr != 0)
               putchar('+');
 }

         putchar('\n');
 }

 else
         display_log(dp, INFORMATION, "%s: skipped known bad combo 0x%x",
            dp->filename, tr);

 return 1;  
 }

 return 0;  
}
