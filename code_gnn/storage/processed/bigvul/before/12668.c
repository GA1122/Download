process_yesno(BOOL skipping, BOOL yes, uschar *save_lookup, uschar **sptr,
  uschar **yieldptr, int *sizeptr, int *ptrptr, uschar *type, BOOL *resetok)
{
int rc = 0;
uschar *s = *sptr;     
uschar *sub1, *sub2;

 

while (isspace(*s)) s++;
if (*s == '}')
  {
  if (type[0] == 'i')
    {
    if (yes) *yieldptr = string_cat(*yieldptr, sizeptr, ptrptr, US"true", 4);
    }
  else
    {
    if (yes && lookup_value != NULL)
      *yieldptr = string_cat(*yieldptr, sizeptr, ptrptr, lookup_value,
        Ustrlen(lookup_value));
    lookup_value = save_lookup;
    }
  s++;
  goto RETURN;
  }

 

if (*s++ != '{') goto FAILED_CURLY;

 

sub1 = expand_string_internal(s, TRUE, &s, !yes, TRUE, resetok);
if (sub1 == NULL && (yes || !expand_string_forcedfail)) goto FAILED;
expand_string_forcedfail = FALSE;
if (*s++ != '}') goto FAILED_CURLY;

 

if (yes)
  *yieldptr = string_cat(*yieldptr, sizeptr, ptrptr, sub1, Ustrlen(sub1));

 

lookup_value = save_lookup;

 

while (isspace(*s)) s++;
if (*s == '{')
  {
  sub2 = expand_string_internal(s+1, TRUE, &s, yes || skipping, TRUE, resetok);
  if (sub2 == NULL && (!yes || !expand_string_forcedfail)) goto FAILED;
  expand_string_forcedfail = FALSE;
  if (*s++ != '}') goto FAILED_CURLY;

   

  if (!yes)
    *yieldptr = string_cat(*yieldptr, sizeptr, ptrptr, sub2, Ustrlen(sub2));
  }

/* If there is no second string, but the word "fail" is present when the use of
the second string is wanted, set a flag indicating it was a forced failure
rather than a syntactic error. Swallow the terminating } in case this is nested
