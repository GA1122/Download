check_variable_error_message(uschar *name)
{
if (Ustrncmp(name, "acl_", 4) == 0)
  expand_string_message = string_sprintf("%s (%s)", expand_string_message,
    (name[4] == 'c' || name[4] == 'm')?
      (isalpha(name[5])?
        US"6th character of a user-defined ACL variable must be a digit or underscore" :
        US"strict_acl_vars is set"     
      ) :
      US"user-defined ACL variables must start acl_c or acl_m");
}
