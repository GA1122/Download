xmlNormalizeURIPath(char *path) {
 char *cur, *out;

 if (path == NULL)
 return(-1);

  
    cur = path;
 while (cur[0] == '/')
 ++cur;
 if (cur[0] == '\0')
 return(0);

  
    out = cur;

  
 while (cur[0] != '\0') {
  
 if ((cur[0] == '.') && (cur[1] == '/')) {
	    cur += 2;
  
 while (cur[0] == '/')
		cur++;
 continue;
 }

  
 if ((cur[0] == '.') && (cur[1] == '\0'))
 break;

  
 while (cur[0] != '/') {
 if (cur[0] == '\0')
 goto done_cd;
 (out++)[0] = (cur++)[0];
 }
  
 while ((cur[0] == '/') && (cur[1] == '/'))
	    cur++;

 (out++)[0] = (cur++)[0];
 }
 done_cd:
    out[0] = '\0';

  
    cur = path;
 while (cur[0] == '/')
 ++cur;
 if (cur[0] == '\0')
 return(0);

  
 while (1) {
 char *segp, *tmp;

  

  
        segp = cur;
 while ((segp[0] != '/') && (segp[0] != '\0'))
 ++segp;

  
 if (segp[0] == '\0')
 break;

  
 ++segp;
 if (((cur[0] == '.') && (cur[1] == '.') && (segp == cur+3))
 || ((segp[0] != '.') || (segp[1] != '.')
 || ((segp[2] != '/') && (segp[2] != '\0')))) {
          cur = segp;
 continue;
 }

  

  
 if (segp[2] == '\0') {
          cur[0] = '\0';
 break;
 }
  
  
        tmp = cur;
        segp += 3;
 while ((*tmp++ = *segp++) != 0)
 ;

  
        segp = cur;
 while ((segp > path) && ((--segp)[0] == '/'))
 ;
 if (segp == path)
 continue;

  
        cur = segp;
 while ((cur > path) && (cur[-1] != '/'))
 --cur;
 }
    out[0] = '\0';

  
 if (path[0] == '/') {
      cur = path;
 while ((cur[0] == '/') && (cur[1] == '.') && (cur[2] == '.')
 && ((cur[3] == '/') || (cur[3] == '\0')))
	cur += 3;

 if (cur != path) {
	out = path;
 while (cur[0] != '\0')
 (out++)[0] = (cur++)[0];
	out[0] = 0;
 }
 }

 return(0);
}
