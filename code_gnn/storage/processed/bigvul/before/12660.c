expand_string_copy(uschar *string)
{
uschar *yield = expand_string(string);
if (yield == string) yield = string_copy(string);
return yield;
}
