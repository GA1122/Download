expand_getcertele(uschar * field, uschar * certvar)
{
var_entry * vp;
certfield * cp;

if (!(vp = find_var_ent(certvar)))
  {
  expand_string_message = 
    string_sprintf("no variable named \"%s\"", certvar);
  return NULL;           
  }
 
if (vp->type != vtype_cert)
  {
  expand_string_message = 
    string_sprintf("\"%s\" is not a certificate", certvar);
  return NULL;           
  }
if (!*(void **)vp->value)
  return NULL;

if (*field >= '0' && *field <= '9')
  return tls_cert_ext_by_oid(*(void **)vp->value, field, 0);

for(cp = certfields;
    cp < certfields + nelements(certfields);
    cp++)
  if (Ustrncmp(cp->name, field, cp->namelen) == 0)
    {
    uschar * modifier = *(field += cp->namelen) == ','
      ? ++field : NULL;
    return (*cp->getfn)( *(void **)vp->value, modifier );
    }

expand_string_message = 
  string_sprintf("bad field selector \"%s\" for certextract", field);
return NULL;
}
