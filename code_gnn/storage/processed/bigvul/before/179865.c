 int X509_cmp_time(const ASN1_TIME *ctm, time_t *cmp_time)
 {
     char *str;
      ASN1_TIME atm;
      long offset;
      char buff1[24], buff2[24], *p;
    int i, j;
//     int i, j, remaining;
  
      p = buff1;
    i = ctm->length;
//     remaining = ctm->length;
      str = (char *)ctm->data;
//      
      if (ctm->type == V_ASN1_UTCTIME) {
        if ((i < 11) || (i > 17))
//          
//         int min_length = sizeof("YYMMDDHHMMZ") - 1;
//         int max_length = sizeof("YYMMDDHHMMSS+hhmm") - 1;
//         if (remaining < min_length || remaining > max_length)
              return 0;
          memcpy(p, str, 10);
          p += 10;
          str += 10;
//         remaining -= 10;
      } else {
        if (i < 13)
//          
//         int min_length = sizeof("YYYYMMDDHHMMZ") - 1;
//         int max_length = sizeof("YYYYMMDDHHMMSS.fff+hhmm") - 1;
//         if (remaining < min_length || remaining > max_length)
              return 0;
          memcpy(p, str, 12);
          p += 12;
          str += 12;
//         remaining -= 12;
      }
  
      if ((*str == 'Z') || (*str == '-') || (*str == '+')) {
          *(p++) = '0';
          *(p++) = '0';
      } else {
//          
//         if (remaining < 2)
//             return 0;
          *(p++) = *(str++);
          *(p++) = *(str++);
//         remaining -= 2;
          
        if (*str == '.') {
//         if (remaining && *str == '.') {
              str++;
            while ((*str >= '0') && (*str <= '9'))
                str++;
//             remaining--;
//             for (i = 0; i < 3 && remaining; i++, str++, remaining--) {
//                 if (*str < '0' || *str > '9')
//                     break;
//             }
          }
  
      }
      *(p++) = 'Z';
      *(p++) = '\0';
  
    if (*str == 'Z')
//      
//     if (!remaining)
//         return 0;
//     if (*str == 'Z') {
//         if (remaining != 1)
//             return 0;
          offset = 0;
    else {
//     } else {
//          
          if ((*str != '+') && (*str != '-'))
              return 0;
//          
//         if (remaining != 5)
//             return 0;
//         if (str[1] < '0' || str[1] > '9' || str[2] < '0' || str[2] > '9' ||
//             str[3] < '0' || str[3] > '9' || str[4] < '0' || str[4] > '9')
//             return 0;
          offset = ((str[1] - '0') * 10 + (str[2] - '0')) * 60;
          offset += (str[3] - '0') * 10 + (str[4] - '0');
          if (*str == '-')
             offset = -offset;
     }
     atm.type = ctm->type;
     atm.flags = 0;
     atm.length = sizeof(buff2);
     atm.data = (unsigned char *)buff2;
 
     if (X509_time_adj(&atm, offset * 60, cmp_time) == NULL)
         return 0;
 
     if (ctm->type == V_ASN1_UTCTIME) {
         i = (buff1[0] - '0') * 10 + (buff1[1] - '0');
         if (i < 50)
             i += 100;            
         j = (buff2[0] - '0') * 10 + (buff2[1] - '0');
         if (j < 50)
             j += 100;
 
         if (i < j)
             return -1;
         if (i > j)
             return 1;
     }
     i = strcmp(buff1, buff2);
     if (i == 0)                  
         return -1;
     else
         return i;
 }