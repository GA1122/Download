static char *get_netscape_format(const struct Cookie *co)
{
  return aprintf(
    "%s"      
    "%s%s\t"  
    "%s\t"    
    "%s\t"    
    "%s\t"    
    "%" FORMAT_OFF_T "\t"    
    "%s\t"    
    "%s",     
    co->httponly?"#HttpOnly_":"",
     
    (co->tailmatch && co->domain && co->domain[0] != '.')? ".":"",
    co->domain?co->domain:"unknown",
    co->tailmatch?"TRUE":"FALSE",
    co->path?co->path:"/",
    co->secure?"TRUE":"FALSE",
    co->expires,
    co->name,
    co->value?co->value:"");
}
