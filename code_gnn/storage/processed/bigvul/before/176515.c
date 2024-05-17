xmlParse3986Authority(xmlURIPtr uri, const char **str)
{
 const char *cur;
 int ret;

    cur = *str;
  
    ret = xmlParse3986Userinfo(uri, &cur);
 if ((ret != 0) || (*cur != '@'))
        cur = *str;
 else
        cur++;
    ret = xmlParse3986Host(uri, &cur);
 if (ret != 0) return(ret);
 if (*cur == ':') {
        cur++;
        ret = xmlParse3986Port(uri, &cur);
 if (ret != 0) return(ret);
 }
 *str = cur;
 return(0);
}
