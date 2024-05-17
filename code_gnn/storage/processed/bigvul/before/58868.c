get_stdinput(char *(*fn_readline)(const char *), void(*fn_addhist)(const char *))
{
int i;
int size = 0;
int ptr = 0;
uschar *yield = NULL;

if (fn_readline == NULL) { printf("> "); fflush(stdout); }

for (i = 0;; i++)
  {
  uschar buffer[1024];
  uschar *p, *ss;

  #ifdef USE_READLINE
  char *readline_line = NULL;
  if (fn_readline != NULL)
    {
    if ((readline_line = fn_readline((i > 0)? "":"> ")) == NULL) break;
    if (*readline_line != 0 && fn_addhist != NULL) fn_addhist(readline_line);
    p = US readline_line;
    }
  else
  #endif

   

    {
    if (Ufgets(buffer, sizeof(buffer), stdin) == NULL) break;
    p = buffer;
    }

   

  ss = p + (int)Ustrlen(p);
  while (ss > p && isspace(ss[-1])) ss--;

  if (i > 0)
    {
    while (p < ss && isspace(*p)) p++;    
    }

  yield = string_catn(yield, &size, &ptr, p, ss - p);

  #ifdef USE_READLINE
  if (fn_readline != NULL) free(readline_line);
  #endif

   
  if (ss == p || yield[ptr-1] != '\\')
    {
    if (yield) yield[ptr] = 0;
    break;
    }
  yield[--ptr] = 0;
  }

if (yield == NULL) printf("\n");
return yield;
}