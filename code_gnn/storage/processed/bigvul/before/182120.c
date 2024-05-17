 void edge_sparse_csr_reader_double( const char*           i_csr_file_in,
                                     unsigned int**        o_row_idx,
                                     unsigned int**        o_column_idx,
                                     double**              o_values,
                                     unsigned int*         o_row_count,
                                     unsigned int*         o_column_count,
                                     unsigned int*         o_element_count ) {
   FILE *l_csr_file_handle;
   const unsigned int l_line_length = 512;
   char l_line[512 +1];
   unsigned int l_header_read = 0;
   unsigned int* l_row_idx_id = NULL;
   unsigned int l_i = 0;
 
   l_csr_file_handle = fopen( i_csr_file_in, "r" );
   if ( l_csr_file_handle == NULL ) {
     fprintf( stderr, "cannot open CSR file!\n" );
     return;
   }
 
   while (fgets(l_line, l_line_length, l_csr_file_handle) != NULL) {
     if ( strlen(l_line) == l_line_length ) {
       fprintf( stderr, "could not read file length!\n" );
       return;
     }
      
     if ( l_line[0] == '%' ) {
       continue;
      } else {
         
        if ( l_header_read == 0 ) {
        if ( sscanf(l_line, "%u %u %u", o_row_count, o_column_count, o_element_count) == 3 ) {
//         if (3 == sscanf(l_line, "%u %u %u", o_row_count, o_column_count, o_element_count) &&
//             0 != *o_row_count && 0 != *o_column_count && 0 != *o_element_count)
//         {
             
            *o_column_idx = (unsigned int*) malloc(sizeof(unsigned int) * (*o_element_count));
            *o_row_idx = (unsigned int*) malloc(sizeof(unsigned int) * (*o_row_count + 1));
           *o_values = (double*) malloc(sizeof(double) * (*o_element_count));
           l_row_idx_id = (unsigned int*) malloc(sizeof(unsigned int) * (*o_row_count));
 
            
           if ( ( *o_row_idx == NULL )      ||
                ( *o_column_idx == NULL )   ||
                ( *o_values == NULL )       ||
                ( l_row_idx_id == NULL ) ) {
             fprintf( stderr, "could not allocate sp data!\n" );
             return;
           }
 
            
           memset(*o_row_idx, 0, sizeof(unsigned int)*(*o_row_count + 1));
           memset(*o_column_idx, 0, sizeof(unsigned int)*(*o_element_count));
           memset(*o_values, 0, sizeof(double)*(*o_element_count));
           memset(l_row_idx_id, 0, sizeof(unsigned int)*(*o_row_count));
 
            
           for ( l_i = 0; l_i < (*o_row_count + 1); l_i++)
             (*o_row_idx)[l_i] = (*o_element_count);
 
            
           (*o_row_idx)[0] = 0;
           l_i = 0;
           l_header_read = 1;
         } else {
           fprintf( stderr, "could not csr description!\n" );
           return;
         }
        
       } else {
         unsigned int l_row, l_column;
         double l_value;
          
         if ( sscanf(l_line, "%u %u %lf", &l_row, &l_column, &l_value) != 3 ) {
           fprintf( stderr, "could not read element!\n" );
           return;
         }
          
         l_row--;
         l_column--;
          
         (*o_column_idx)[l_i] = l_column;
         (*o_values)[l_i] = l_value;
         l_i++;
          
         l_row_idx_id[l_row] = 1;
         (*o_row_idx)[l_row+1] = l_i;
       }
     }
   }
 
    
   fclose( l_csr_file_handle );
 
    
   if ( l_i != (*o_element_count) ) {
     fprintf( stderr, "we were not able to read all elements!\n" );
     return;
   }
 
    
   for ( l_i = 0; l_i < (*o_row_count); l_i++) {
     if ( l_row_idx_id[l_i] == 0 ) {
       (*o_row_idx)[l_i+1] = (*o_row_idx)[l_i];
     }
   }
 
    
   if ( l_row_idx_id != NULL ) {
     free( l_row_idx_id );
   }
 }