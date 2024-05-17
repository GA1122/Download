static int test_weird_arguments(void)
{
  int errors = 0;
  char buf[256];
  int rc;

   
  rc = curl_msnprintf(buf, sizeof(buf),
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d"      
                      ,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7);  

  if(rc != 128) {
    printf("curl_mprintf() returned %d and not 128!\n", rc);
    errors++;
  }

  errors += string_check(buf,
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "0123456789"  
                         "01234567"    
    );

   
  buf[0] = 0;
  rc = curl_msnprintf(buf, sizeof(buf),
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d%d"  
                      "%d%d%d%d%d%d%d%d%d"    
                      ,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  
                      0, 1, 2, 3, 4, 5, 6, 7, 8);    

  if(rc != -1) {
    printf("curl_mprintf() returned %d and not -1!\n", rc);
    errors++;
  }

  errors += string_check(buf, "");

  if(errors)
    printf("Some curl_mprintf() weird arguments tests failed!\n");

   return errors;
 }
