xmlParse3986Segment(const char **str, char forbid, int empty)
{
 const char *cur;

    cur = *str;
 if (!ISA_PCHAR(cur)) {
 if (empty)
 return(0);
 return(1);
 }
 while (ISA_PCHAR(cur) && (*cur != forbid))
        NEXT(cur);
 *str = cur;
 return (0);
}
