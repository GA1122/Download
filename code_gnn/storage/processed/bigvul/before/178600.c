   T1_Get_Private_Dict( T1_Parser      parser,
                        PSAux_Service  psaux )
   {
     FT_Stream  stream = parser->stream;
     FT_Memory  memory = parser->root.memory;
     FT_Error   error  = FT_Err_Ok;
     FT_ULong   size;
 
 
     if ( parser->in_pfb )
     {
        
        
        
        
       FT_ULong   start_pos = FT_STREAM_POS();
       FT_UShort  tag;
 
 
       parser->private_len = 0;
       for (;;)
       {
         error = read_pfb_tag( stream, &tag, &size );
         if ( error )
           goto Fail;
 
         if ( tag != 0x8002U )
           break;
 
         parser->private_len += size;
 
         if ( FT_STREAM_SKIP( size ) )
           goto Fail;
       }
 
        
        
       if ( parser->private_len == 0 )
       {
         FT_ERROR(( "T1_Get_Private_Dict:"
                    " invalid private dictionary section\n" ));
         error = FT_THROW( Invalid_File_Format );
         goto Fail;
       }
 
       if ( FT_STREAM_SEEK( start_pos )                           ||
            FT_ALLOC( parser->private_dict, parser->private_len ) )
         goto Fail;
 
       parser->private_len = 0;
       for (;;)
       {
         error = read_pfb_tag( stream, &tag, &size );
         if ( error || tag != 0x8002U )
         {
           error = FT_Err_Ok;
           break;
         }
 
         if ( FT_STREAM_READ( parser->private_dict + parser->private_len,
                              size ) )
           goto Fail;
 
         parser->private_len += size;
       }
     }
     else
     {
        
        
        
        
 
        
       FT_Byte*    cur   = parser->base_dict;
       FT_Byte*    limit = cur + parser->base_len;
       FT_Byte     c;
       FT_Pointer  pos_lf;
       FT_Bool     test_cr;
 
 
     Again:
       for (;;)
       {
         c = cur[0];
         if ( c == 'e' && cur + 9 < limit )   
                                              
         {
           if ( cur[1] == 'e' &&
                cur[2] == 'x' &&
                cur[3] == 'e' &&
                cur[4] == 'c' )
             break;
         }
         cur++;
         if ( cur >= limit )
         {
           FT_ERROR(( "T1_Get_Private_Dict:"
                      " could not find `eexec' keyword\n" ));
           error = FT_THROW( Invalid_File_Format );
           goto Exit;
         }
       }
 
        
        
 
       parser->root.cursor = parser->base_dict;
        
       parser->root.limit  = cur + 10;
 
       cur   = parser->root.cursor;
       limit = parser->root.limit;
 
       while ( cur < limit )
       {
         if ( *cur == 'e' && ft_strncmp( (char*)cur, "eexec", 5 ) == 0 )
           goto Found;
 
         T1_Skip_PS_Token( parser );
         if ( parser->root.error )
           break;
         T1_Skip_Spaces  ( parser );
         cur = parser->root.cursor;
       }
 
        
        
  
        cur   = limit;
        limit = parser->base_dict + parser->base_len;
// 
//       if ( cur >= limit )
//       {
//         FT_ERROR(( "T1_Get_Private_Dict:"
//                    " premature end in private dictionary\n" ));
//         error = FT_THROW( Invalid_File_Format );
//         goto Exit;
//       }
// 
        goto Again;
  
         
 
        
        
        
        
        
        
        
        
 
       pos_lf  = ft_memchr( cur, '\n', (size_t)( limit - cur ) );
       test_cr = FT_BOOL( !pos_lf                                       ||
                          pos_lf > ft_memchr( cur,
                                              '\r',
                                              (size_t)( limit - cur ) ) );
 
       while ( cur < limit                    &&
               ( *cur == ' '                ||
                 *cur == '\t'               ||
                 (test_cr && *cur == '\r' ) ||
                 *cur == '\n'               ) )
         ++cur;
       if ( cur >= limit )
       {
         FT_ERROR(( "T1_Get_Private_Dict:"
                    " `eexec' not properly terminated\n" ));
         error = FT_THROW( Invalid_File_Format );
         goto Exit;
       }
 
       size = parser->base_len - (FT_ULong)( cur - parser->base_dict );
 
       if ( parser->in_memory )
       {
          
         if ( FT_ALLOC( parser->private_dict, size + 1 ) )
           goto Fail;
         parser->private_len = size;
       }
       else
       {
         parser->single_block = 1;
         parser->private_dict = parser->base_dict;
         parser->private_len  = size;
         parser->base_dict    = NULL;
         parser->base_len     = 0;
       }
 
        
        
 
        
        
        
 
       if ( cur + 3 < limit                                &&
            ft_isxdigit( cur[0] ) && ft_isxdigit( cur[1] ) &&
            ft_isxdigit( cur[2] ) && ft_isxdigit( cur[3] ) )
       {
          
         FT_ULong  len;
 
 
         parser->root.cursor = cur;
         (void)psaux->ps_parser_funcs->to_bytes( &parser->root,
                                                 parser->private_dict,
                                                 parser->private_len,
                                                 &len,
                                                 0 );
         parser->private_len = len;
 
          
         parser->private_dict[len] = '\0';
       }
       else
          
         FT_MEM_MOVE( parser->private_dict, cur, size );
     }
 
      
     psaux->t1_decrypt( parser->private_dict, parser->private_len, 55665U );
 
     if ( parser->private_len < 4 )
     {
       FT_ERROR(( "T1_Get_Private_Dict:"
                  " invalid private dictionary section\n" ));
       error = FT_THROW( Invalid_File_Format );
       goto Fail;
     }
 
      
     parser->private_dict[0] = ' ';
     parser->private_dict[1] = ' ';
     parser->private_dict[2] = ' ';
     parser->private_dict[3] = ' ';
 
     parser->root.base   = parser->private_dict;
     parser->root.cursor = parser->private_dict;
     parser->root.limit  = parser->root.cursor + parser->private_len;
 
   Fail:
   Exit:
     return error;
   }