static char *get_skip_str(int *skip_val)
{
   char *start_skip = NULL;
   if (*skip_val < 0) {
      *skip_val = 0;
      return NULL;
   }

   if (*skip_val == 1) {
      start_skip = strdup("gl_SkipComponents1");
      *skip_val -= 1;
   } else if (*skip_val == 2) {
      start_skip = strdup("gl_SkipComponents2");
      *skip_val -= 2;
   } else if (*skip_val == 3) {
      start_skip = strdup("gl_SkipComponents3");
      *skip_val -= 3;
   } else if (*skip_val >= 4) {
      start_skip = strdup("gl_SkipComponents4");
      *skip_val -= 4;
   }
   return start_skip;
}
