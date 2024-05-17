 main (int argc _GL_UNUSED, char **argv)
 {
   struct timespec result;
   struct timespec result2;
   struct timespec expected;
   struct timespec now;
   const char *p;
   int i;
   long gmtoff;
   time_t ref_time = 1304250918;
 
    
   setenv ("TZ", "EST5EDT,M3.2.0,M11.1.0", 1);
 
   gmtoff = gmt_offset (ref_time);
 
 
    
   p = "2011-05-01T11:55:18";
   expected.tv_sec = ref_time - gmtoff;
   expected.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, 0));
   LOG (p, expected, result);
   ASSERT (expected.tv_sec == result.tv_sec
           && expected.tv_nsec == result.tv_nsec);
 
    
   p = "2011-05-01 11:55:18";
   expected.tv_sec = ref_time - gmtoff;
   expected.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, 0));
   LOG (p, expected, result);
   ASSERT (expected.tv_sec == result.tv_sec
           && expected.tv_nsec == result.tv_nsec);
 
 
    
   p = "2011-05-01T11:55:18Z";
   expected.tv_sec = ref_time;
   expected.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, 0));
   LOG (p, expected, result);
   ASSERT (expected.tv_sec == result.tv_sec
           && expected.tv_nsec == result.tv_nsec);
 
    
   p = "2011-05-01 11:55:18Z";
   expected.tv_sec = ref_time;
   expected.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, 0));
   LOG (p, expected, result);
   ASSERT (expected.tv_sec == result.tv_sec
           && expected.tv_nsec == result.tv_nsec);
 
 
    
   p = "2011-05-01T11:55:18-07:00";
   expected.tv_sec = 1304276118;
   expected.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, 0));
   LOG (p, expected, result);
   ASSERT (expected.tv_sec == result.tv_sec
           && expected.tv_nsec == result.tv_nsec);
 
    
   p = "2011-05-01 11:55:18-07:00";
   expected.tv_sec = 1304276118;
   expected.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, 0));
   LOG (p, expected, result);
   ASSERT (expected.tv_sec == result.tv_sec
           && expected.tv_nsec == result.tv_nsec);
 
 
    
   p = "2011-05-01T11:55:18-07";
   expected.tv_sec = 1304276118;
   expected.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, 0));
   LOG (p, expected, result);
   ASSERT (expected.tv_sec == result.tv_sec
           && expected.tv_nsec == result.tv_nsec);
 
    
   p = "2011-05-01 11:55:18-07";
   expected.tv_sec = 1304276118;
   expected.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, 0));
   LOG (p, expected, result);
   ASSERT (expected.tv_sec == result.tv_sec
           && expected.tv_nsec == result.tv_nsec);
 
 
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "now";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   ASSERT (now.tv_sec == result.tv_sec && now.tv_nsec == result.tv_nsec);
 
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "tomorrow";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   ASSERT (now.tv_sec + 24 * 60 * 60 == result.tv_sec
           && now.tv_nsec == result.tv_nsec);
 
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "yesterday";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   ASSERT (now.tv_sec - 24 * 60 * 60 == result.tv_sec
           && now.tv_nsec == result.tv_nsec);
 
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "4 hours";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   ASSERT (now.tv_sec + 4 * 60 * 60 == result.tv_sec
           && now.tv_nsec == result.tv_nsec);
 
    
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC+400 +24 hours";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   p = "UTC+400 +1 day";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
 
    
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC+14:00";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   p = "UTC+14";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
   p = "UTC+1400";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
 
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC-14:00";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   p = "UTC-14";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
   p = "UTC-1400";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
 
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC+0:15";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   p = "UTC+0015";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
 
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC-1:30";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   p = "UTC-130";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
 
 
    
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC+25:00";
   ASSERT (!parse_datetime (&result, p, &now));
 
          
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC+4:00 +40 yesterday";
   ASSERT (!parse_datetime (&result, p, &now));
   p = "UTC+4:00 next yesterday";
   ASSERT (!parse_datetime (&result, p, &now));
   p = "UTC+4:00 tomorrow ago";
   ASSERT (!parse_datetime (&result, p, &now));
   p = "UTC+4:00 tomorrow hence";
   ASSERT (!parse_datetime (&result, p, &now));
   p = "UTC+4:00 40 now ago";
   ASSERT (!parse_datetime (&result, p, &now));
   p = "UTC+4:00 last tomorrow";
   ASSERT (!parse_datetime (&result, p, &now));
   p = "UTC+4:00 -4 today";
   ASSERT (!parse_datetime (&result, p, &now));
 
    
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC+400 tomorrow";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   p = "UTC+400 +1 day";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
   p = "UTC+400 1 day hence";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC+400 yesterday";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   p = "UTC+400 1 day ago";
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
   now.tv_sec = 4711;
   now.tv_nsec = 1267;
   p = "UTC+400 now";
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   p = "UTC+400 +0 minutes";  
   ASSERT (parse_datetime (&result2, p, &now));
   LOG (p, now, result2);
   ASSERT (result.tv_sec == result2.tv_sec
           && result.tv_nsec == result2.tv_nsec);
 
    
   setenv ("TZ", "UTC0", 1);
   for (i = 0; day_table[i]; i++)
     {
       unsigned int thur2 = 7 * 24 * 3600;  
       char tmp[32];
       sprintf (tmp, "NEXT %s", day_table[i]);
       now.tv_sec = thur2 + 4711;
       now.tv_nsec = 1267;
       ASSERT (parse_datetime (&result, tmp, &now));
       LOG (tmp, now, result);
       ASSERT (result.tv_nsec == 0);
       ASSERT (result.tv_sec == thur2 + (i == 4 ? 7 : (i + 3) % 7) * 24 * 3600);
 
       sprintf (tmp, "LAST %s", day_table[i]);
       now.tv_sec = thur2 + 4711;
       now.tv_nsec = 1267;
       ASSERT (parse_datetime (&result, tmp, &now));
       LOG (tmp, now, result);
       ASSERT (result.tv_nsec == 0);
       ASSERT (result.tv_sec == thur2 + ((i + 3) % 7 - 7) * 24 * 3600);
     }
 
   p = "THURSDAY UTC+00";   
   now.tv_sec = 0;
   now.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   ASSERT (result.tv_sec == now.tv_sec
           && result.tv_nsec == now.tv_nsec);
 
   p = "FRIDAY UTC+00";
   now.tv_sec = 0;
   now.tv_nsec = 0;
   ASSERT (parse_datetime (&result, p, &now));
   LOG (p, now, result);
   ASSERT (result.tv_sec == 24 * 3600
           && result.tv_nsec == now.tv_nsec);
 
    
   ASSERT ( ! parse_datetime (&result, "\xb0", &now));
 
    
    
   ASSERT ( ! parse_datetime (&result, "TZ=\"\"\"", &now));
   ASSERT ( ! parse_datetime (&result, "TZ=\"\" \"", &now));
    
   ASSERT ( ! parse_datetime (&result, "TZ=\"", &now));
   ASSERT ( ! parse_datetime (&result, "TZ=\"\\\"", &now));
   ASSERT ( ! parse_datetime (&result, "TZ=\"\\n", &now));
   ASSERT ( ! parse_datetime (&result, "TZ=\"\\n\"", &now));
    
   ASSERT (   parse_datetime (&result, "TZ=\"\"", &now));
   ASSERT (   parse_datetime (&result, "TZ=\"\" ", &now));
   ASSERT (   parse_datetime (&result, " TZ=\"\"", &now));
    ASSERT (   parse_datetime (&result, "TZ=\"\\\\\"", &now));
    ASSERT (   parse_datetime (&result, "TZ=\"\\\"\"", &now));
  
//    
//   {
//     static char const bufprefix[] = "TZ=\"";
//     enum { tzname_len = 2000 };
//     static char const bufsuffix[] = "0\" 1970-01-01 01:02:03.123456789";
//     enum { bufsize = sizeof bufprefix - 1 + tzname_len + sizeof bufsuffix };
//     char buf[bufsize];
//     memcpy (buf, bufprefix, sizeof bufprefix - 1);
//     memset (buf + sizeof bufprefix - 1, 'X', tzname_len);
//     strcpy (buf + bufsize - sizeof bufsuffix, bufsuffix);
//     ASSERT (parse_datetime (&result, buf, &now));
//     LOG (buf, now, result);
//     ASSERT (result.tv_sec == 1 * 60 * 60 + 2 * 60 + 3
//             && result.tv_nsec == 123456789);
//   }
// 
    return 0;
  }