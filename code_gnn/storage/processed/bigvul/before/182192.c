 static void t1_check_unusual_charstring(void)
 {
     char *p = strstr(t1_line_array, charstringname) + strlen(charstringname);
     int i;
      
     if (sscanf(p, "%i", &i) != 1) {
          
         strcpy(t1_buf_array, t1_line_array);
 
          
          *(strend(t1_buf_array) - 1) = ' ';
  
          t1_getline();
//         alloc_array(t1_buf, strlen(t1_line_array) + strlen(t1_buf_array) + 1, T1_BUF_SIZE);
          strcat(t1_buf_array, t1_line_array);
//         alloc_array(t1_line, strlen(t1_buf_array) + 1, T1_BUF_SIZE);
          strcpy(t1_line_array, t1_buf_array);
          t1_line_ptr = eol(t1_line_array);
      }
 }