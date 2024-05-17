int str16eq(const uint16_t *a, const char *b)
{
 while (*a && *b)
 if (*a++ != *b++) return 0;
 if (*a || *b)
 return 0;
 return 1;
}
