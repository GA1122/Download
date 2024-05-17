sha1_says_nonexistent(char const *sha1, char const *end)
{
  char const *empty_sha1 = "e69de29bb2d1d6434b8b29ae775ad8c2e48c5391";
  char const *s;

   
  for (s = sha1; s != end; s++)
    if (*s != '0')
      break;
  if (s == end)
    return 2;

   
  for (s = sha1; s != end; s++, empty_sha1++)
    if (*s != *empty_sha1)
      break;
  return s == end;
}
