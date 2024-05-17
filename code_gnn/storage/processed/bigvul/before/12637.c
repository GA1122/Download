cat_file(FILE *f, uschar *yield, int *sizep, int *ptrp, uschar *eol)
{
int eollen;
uschar buffer[1024];

eollen = (eol == NULL)? 0 : Ustrlen(eol);

while (Ufgets(buffer, sizeof(buffer), f) != NULL)
  {
  int len = Ustrlen(buffer);
  if (eol != NULL && buffer[len-1] == '\n') len--;
  yield = string_cat(yield, sizep, ptrp, buffer, len);
  if (buffer[len] != 0)
    yield = string_cat(yield, sizep, ptrp, eol, eollen);
  }

if (yield != NULL) yield[*ptrp] = 0;

return yield;
}
