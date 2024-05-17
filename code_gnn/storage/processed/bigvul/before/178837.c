 char *curl_easy_unescape(CURL *handle, const char *string, int length,
                         int *olen)
// CURLcode Curl_urldecode(struct SessionHandle *data,
//                         const char *string, size_t length,
//                         char **ostring, size_t *olen,
//                         bool reject_ctrl)
  {
  int alloc = (length?length:(int)strlen(string))+1;
//   size_t alloc = (length?length:strlen(string))+1;
    char *ns = malloc(alloc);
    unsigned char in;
  int strindex=0;
//   size_t strindex=0;
    unsigned long hex;
    CURLcode res;
  
    if(!ns)
    return NULL;
//     return CURLE_OUT_OF_MEMORY;
  
    while(--alloc > 0) {
      in = *string;
     if(('%' == in) && ISXDIGIT(string[1]) && ISXDIGIT(string[2])) {
        
       char hexstr[3];
       char *ptr;
       hexstr[0] = string[1];
       hexstr[1] = string[2];
       hexstr[2] = 0;
 
       hex = strtoul(hexstr, &ptr, 16);
  
        in = curlx_ultouc(hex);  
  
      res = Curl_convert_from_network(handle, &in, 1);
//       res = Curl_convert_from_network(data, &in, 1);
        if(res) {
           
          free(ns);
        return NULL;
//         return res;
        }
  
        string+=2;
        alloc-=2;
      }
//     if(reject_ctrl && (in < 0x20)) {
//       free(ns);
//       return CURLE_URL_MALFORMAT;
//     }
  
      ns[strindex++] = in;
      string++;
   }
   ns[strindex]=0;  
 
    if(olen)
       
      *olen = strindex;
  return ns;
// 
//   if(ostring)
//      
//     *ostring = ns;
// 
//   return CURLE_OK;
// }
// 
//  
// char *curl_easy_unescape(CURL *handle, const char *string, int length,
//                          int *olen)
// {
//   char *str = NULL;
//   size_t inputlen = length;
//   size_t outputlen;
//   CURLcode res = Curl_urldecode(handle, string, inputlen, &str, &outputlen,
//                                 FALSE);
//   if(res)
//     return NULL;
//   if(olen)
//     *olen = curlx_uztosi(outputlen);
//   return str;
  }