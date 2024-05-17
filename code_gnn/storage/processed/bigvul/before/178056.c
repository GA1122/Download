   t1_decoder_parse_charstrings( T1_Decoder  decoder,
                                 FT_Byte*    charstring_base,
                                 FT_UInt     charstring_len )
   {
     FT_Error         error;
     T1_Decoder_Zone  zone;
     FT_Byte*         ip;
     FT_Byte*         limit;
     T1_Builder       builder = &decoder->builder;
     FT_Pos           x, y, orig_x, orig_y;
     FT_Int           known_othersubr_result_cnt   = 0;
     FT_Int           unknown_othersubr_result_cnt = 0;
     FT_Bool          large_int;
     FT_Fixed         seed;
 
     T1_Hints_Funcs   hinter;
 
 #ifdef FT_DEBUG_LEVEL_TRACE
     FT_Bool          bol = TRUE;
 #endif
 
 
      
     seed = (FT_Fixed)( ( (FT_Offset)(char*)&seed            ^
                          (FT_Offset)(char*)&decoder         ^
                          (FT_Offset)(char*)&charstring_base ) &
                          FT_ULONG_MAX                         );
     seed = ( seed ^ ( seed >> 10 ) ^ ( seed >> 20 ) ) & 0xFFFFL;
     if ( seed == 0 )
       seed = 0x7384;
 
      
     decoder->top  = decoder->stack;
     decoder->zone = decoder->zones;
     zone          = decoder->zones;
 
     builder->parse_state = T1_Parse_Start;
 
     hinter = (T1_Hints_Funcs)builder->hints_funcs;
 
      
      
     FT_ASSERT( ( decoder->len_buildchar == 0 ) ==
                ( decoder->buildchar == NULL )  );
 
     if ( decoder->buildchar && decoder->len_buildchar > 0 )
       FT_ARRAY_ZERO( decoder->buildchar, decoder->len_buildchar );
 
     FT_TRACE4(( "\n"
                 "Start charstring\n" ));
 
     zone->base           = charstring_base;
     limit = zone->limit  = charstring_base + charstring_len;
     ip    = zone->cursor = zone->base;
 
     error = FT_Err_Ok;
 
     x = orig_x = builder->pos_x;
     y = orig_y = builder->pos_y;
 
      
     if ( hinter )
       hinter->open( hinter->hints );
 
     large_int = FALSE;
 
      
     while ( ip < limit )
     {
       FT_Long*     top   = decoder->top;
       T1_Operator  op    = op_none;
       FT_Int32     value = 0;
 
 
       FT_ASSERT( known_othersubr_result_cnt == 0   ||
                  unknown_othersubr_result_cnt == 0 );
 
 #ifdef FT_DEBUG_LEVEL_TRACE
       if ( bol )
       {
         FT_TRACE5(( " (%d)", decoder->top - decoder->stack ));
         bol = FALSE;
       }
 #endif
 
        
        
        
        
        
 
        
       switch ( *ip++ )
       {
       case 1:
         op = op_hstem;
         break;
 
       case 3:
         op = op_vstem;
         break;
       case 4:
         op = op_vmoveto;
         break;
       case 5:
         op = op_rlineto;
         break;
       case 6:
         op = op_hlineto;
         break;
       case 7:
         op = op_vlineto;
         break;
       case 8:
         op = op_rrcurveto;
         break;
       case 9:
         op = op_closepath;
         break;
       case 10:
         op = op_callsubr;
         break;
       case 11:
         op = op_return;
         break;
 
       case 13:
         op = op_hsbw;
         break;
       case 14:
         op = op_endchar;
         break;
 
       case 15:           
         op = op_unknown15;
         break;
 
       case 21:
         op = op_rmoveto;
         break;
       case 22:
         op = op_hmoveto;
         break;
 
       case 30:
         op = op_vhcurveto;
         break;
       case 31:
         op = op_hvcurveto;
         break;
 
       case 12:
         if ( ip >= limit )
         {
           FT_ERROR(( "t1_decoder_parse_charstrings:"
                      " invalid escape (12+EOF)\n" ));
           goto Syntax_Error;
         }
 
         switch ( *ip++ )
         {
         case 0:
           op = op_dotsection;
           break;
         case 1:
           op = op_vstem3;
           break;
         case 2:
           op = op_hstem3;
           break;
         case 6:
           op = op_seac;
           break;
         case 7:
           op = op_sbw;
           break;
         case 12:
           op = op_div;
           break;
         case 16:
           op = op_callothersubr;
           break;
         case 17:
           op = op_pop;
           break;
         case 33:
           op = op_setcurrentpoint;
           break;
 
         default:
           FT_ERROR(( "t1_decoder_parse_charstrings:"
                      " invalid escape (12+%d)\n",
                      ip[-1] ));
           goto Syntax_Error;
         }
         break;
 
       case 255:     
         if ( ip + 4 > limit )
         {
           FT_ERROR(( "t1_decoder_parse_charstrings:"
                      " unexpected EOF in integer\n" ));
           goto Syntax_Error;
         }
 
         value = (FT_Int32)( ( (FT_UInt32)ip[0] << 24 ) |
                             ( (FT_UInt32)ip[1] << 16 ) |
                             ( (FT_UInt32)ip[2] << 8  ) |
                               (FT_UInt32)ip[3]         );
         ip += 4;
 
          
          
          
          
          
          
          
         if ( value > 32000 || value < -32000 )
         {
           if ( large_int )
           {
             FT_ERROR(( "t1_decoder_parse_charstrings:"
                        " no `div' after large integer\n" ));
           }
           else
             large_int = TRUE;
         }
         else
         {
           if ( !large_int )
             value = (FT_Int32)( (FT_UInt32)value << 16 );
         }
 
         break;
 
       default:
         if ( ip[-1] >= 32 )
         {
           if ( ip[-1] < 247 )
             value = (FT_Int32)ip[-1] - 139;
           else
           {
             if ( ++ip > limit )
             {
               FT_ERROR(( "t1_decoder_parse_charstrings:"
                          " unexpected EOF in integer\n" ));
               goto Syntax_Error;
             }
 
             if ( ip[-2] < 251 )
               value =    ( ( ip[-2] - 247 ) * 256 ) + ip[-1] + 108;
             else
               value = -( ( ( ip[-2] - 251 ) * 256 ) + ip[-1] + 108 );
           }
 
           if ( !large_int )
             value = (FT_Int32)( (FT_UInt32)value << 16 );
         }
         else
         {
           FT_ERROR(( "t1_decoder_parse_charstrings:"
                      " invalid byte (%d)\n", ip[-1] ));
           goto Syntax_Error;
         }
       }
 
       if ( unknown_othersubr_result_cnt > 0 )
       {
         switch ( op )
         {
         case op_callsubr:
         case op_return:
         case op_none:
         case op_pop:
           break;
 
         default:
            
           unknown_othersubr_result_cnt = 0;
           break;
         }
       }
 
       if ( large_int && !( op == op_none || op == op_div ) )
       {
         FT_ERROR(( "t1_decoder_parse_charstrings:"
                    " no `div' after large integer\n" ));
 
         large_int = FALSE;
       }
 
        
        
        
        
        
       if ( op == op_none )
       {
         if ( top - decoder->stack >= T1_MAX_CHARSTRINGS_OPERANDS )
         {
           FT_ERROR(( "t1_decoder_parse_charstrings: stack overflow\n" ));
           goto Syntax_Error;
         }
 
 #ifdef FT_DEBUG_LEVEL_TRACE
         if ( large_int )
           FT_TRACE4(( " %d", value ));
         else
           FT_TRACE4(( " %d", value / 65536 ));
 #endif
 
         *top++       = value;
         decoder->top = top;
       }
       else if ( op == op_callothersubr )   
       {
         FT_Int  subr_no;
         FT_Int  arg_cnt;
 
 
 #ifdef FT_DEBUG_LEVEL_TRACE
         FT_TRACE4(( " callothersubr\n" ));
         bol = TRUE;
 #endif
 
         if ( top - decoder->stack < 2 )
           goto Stack_Underflow;
 
         top -= 2;
 
         subr_no = Fix2Int( top[1] );
         arg_cnt = Fix2Int( top[0] );
 
          
          
          
          
          
          
          
          
          
          
 
         if ( arg_cnt > top - decoder->stack )
           goto Stack_Underflow;
 
         top -= arg_cnt;
 
         known_othersubr_result_cnt   = 0;
         unknown_othersubr_result_cnt = 0;
 
          
          
          
          
          
          
          
          
          
          
          
          
          
          
          
 
         switch ( subr_no )
         {
         case 0:                      
           if ( arg_cnt != 3 )
             goto Unexpected_OtherSubr;
 
           if ( !decoder->flex_state           ||
                decoder->num_flex_vectors != 7 )
           {
             FT_ERROR(( "t1_decoder_parse_charstrings:"
                        " unexpected flex end\n" ));
             goto Syntax_Error;
           }
 
            
           top[0] = x;
           top[1] = y;
           known_othersubr_result_cnt = 2;
           break;
 
         case 1:                      
           if ( arg_cnt != 0 )
             goto Unexpected_OtherSubr;
 
           if ( FT_SET_ERROR( t1_builder_start_point( builder, x, y ) ) ||
                FT_SET_ERROR( t1_builder_check_points( builder, 6 ) )   )
             goto Fail;
 
           decoder->flex_state        = 1;
           decoder->num_flex_vectors  = 0;
           break;
 
         case 2:                      
           {
             FT_Int  idx;
 
 
             if ( arg_cnt != 0 )
               goto Unexpected_OtherSubr;
 
             if ( !decoder->flex_state )
             {
               FT_ERROR(( "t1_decoder_parse_charstrings:"
                          " missing flex start\n" ));
               goto Syntax_Error;
             }
 
              
              
               
              idx = decoder->num_flex_vectors++;
              if ( idx > 0 && idx < 7 )
//             {
//                
//                
//                
//                
//               if ( FT_SET_ERROR( t1_builder_check_points( builder, 1 ) ) )
//                 goto Syntax_Error;
// 
                t1_builder_add_point( builder,
                                      x,
                                      y,
                                      (FT_Byte)( idx == 3 || idx == 6 ) );
//             }
            }
            break;
           break;
 
         case 12:
         case 13:
            
           top = decoder->stack;
           break;
 
         case 14:
         case 15:
         case 16:
         case 17:
         case 18:                     
           {
             PS_Blend  blend = decoder->blend;
             FT_UInt   num_points, nn, mm;
             FT_Long*  delta;
             FT_Long*  values;
 
 
             if ( !blend )
             {
               FT_ERROR(( "t1_decoder_parse_charstrings:"
                          " unexpected multiple masters operator\n" ));
               goto Syntax_Error;
             }
 
             num_points = (FT_UInt)subr_no - 13 + ( subr_no == 18 );
             if ( arg_cnt != (FT_Int)( num_points * blend->num_designs ) )
             {
               FT_ERROR(( "t1_decoder_parse_charstrings:"
                          " incorrect number of multiple masters arguments\n" ));
               goto Syntax_Error;
             }
 
              
              
              
              
              
              
              
              
              
              
              
              
              
              
              
              
             delta  = top + num_points;
             values = top;
             for ( nn = 0; nn < num_points; nn++ )
             {
               FT_Long  tmp = values[0];
 
 
               for ( mm = 1; mm < blend->num_designs; mm++ )
                 tmp += FT_MulFix( *delta++, blend->weight_vector[mm] );
 
               *values++ = tmp;
             }
 
             known_othersubr_result_cnt = (FT_Int)num_points;
             break;
           }
 
         case 19:
            
            
            
           {
             FT_Int    idx;
             PS_Blend  blend = decoder->blend;
 
 
             if ( arg_cnt != 1 || !blend )
               goto Unexpected_OtherSubr;
 
             idx = Fix2Int( top[0] );
 
             if ( idx < 0                                                    ||
                  (FT_UInt)idx + blend->num_designs > decoder->len_buildchar )
               goto Unexpected_OtherSubr;
 
             ft_memcpy( &decoder->buildchar[idx],
                        blend->weight_vector,
                        blend->num_designs *
                          sizeof ( blend->weight_vector[0] ) );
           }
           break;
 
         case 20:
            
            
           if ( arg_cnt != 2 )
             goto Unexpected_OtherSubr;
 
           top[0] += top[1];  
 
           known_othersubr_result_cnt = 1;
           break;
 
         case 21:
            
            
           if ( arg_cnt != 2 )
             goto Unexpected_OtherSubr;
 
           top[0] -= top[1];  
 
           known_othersubr_result_cnt = 1;
           break;
 
         case 22:
            
            
           if ( arg_cnt != 2 )
             goto Unexpected_OtherSubr;
 
           top[0] = FT_MulFix( top[0], top[1] );
 
           known_othersubr_result_cnt = 1;
           break;
 
         case 23:
            
            
           if ( arg_cnt != 2 || top[1] == 0 )
             goto Unexpected_OtherSubr;
 
           top[0] = FT_DivFix( top[0], top[1] );
 
           known_othersubr_result_cnt = 1;
           break;
 
         case 24:
            
            
           {
             FT_Int    idx;
             PS_Blend  blend = decoder->blend;
 
 
             if ( arg_cnt != 2 || !blend )
               goto Unexpected_OtherSubr;
 
             idx = Fix2Int( top[1] );
 
             if ( idx < 0 || (FT_UInt) idx >= decoder->len_buildchar )
               goto Unexpected_OtherSubr;
 
             decoder->buildchar[idx] = top[0];
           }
           break;
 
         case 25:
            
            
            
           {
             FT_Int    idx;
             PS_Blend  blend = decoder->blend;
 
 
             if ( arg_cnt != 1 || !blend )
               goto Unexpected_OtherSubr;
 
             idx = Fix2Int( top[0] );
 
             if ( idx < 0 || (FT_UInt) idx >= decoder->len_buildchar )
               goto Unexpected_OtherSubr;
 
             top[0] = decoder->buildchar[idx];
           }
 
           known_othersubr_result_cnt = 1;
           break;
 
 #if 0
         case 26:
            
            
            
           XXX which routine has left its mark on the (PostScript) stack?;
           break;
 #endif
 
         case 27:
            
            
            
           if ( arg_cnt != 4 )
             goto Unexpected_OtherSubr;
 
           if ( top[2] > top[3] )
             top[0] = top[1];
 
           known_othersubr_result_cnt = 1;
           break;
 
         case 28:
            
            
           if ( arg_cnt != 0 )
             goto Unexpected_OtherSubr;
 
           {
             FT_Fixed  Rand;
 
 
             Rand = seed;
             if ( Rand >= 0x8000L )
               Rand++;
 
             top[0] = Rand;
 
             seed = FT_MulFix( seed, 0x10000L - seed );
             if ( seed == 0 )
               seed += 0x2873;
           }
 
           known_othersubr_result_cnt = 1;
           break;
 
         default:
           if ( arg_cnt >= 0 && subr_no >= 0 )
           {
             FT_ERROR(( "t1_decoder_parse_charstrings:"
                        " unknown othersubr [%d %d], wish me luck\n",
                        arg_cnt, subr_no ));
             unknown_othersubr_result_cnt = arg_cnt;
             break;
           }
            
 
         Unexpected_OtherSubr:
           FT_ERROR(( "t1_decoder_parse_charstrings:"
                      " invalid othersubr [%d %d]\n", arg_cnt, subr_no ));
           goto Syntax_Error;
         }
 
         top += known_othersubr_result_cnt;
 
         decoder->top = top;
       }
       else   
       {
         FT_Int  num_args = t1_args_count[op];
 
 
         FT_ASSERT( num_args >= 0 );
 
         if ( top - decoder->stack < num_args )
           goto Stack_Underflow;
 
          
          
          
          
          
 
 #ifdef FT_DEBUG_LEVEL_TRACE
 
         switch ( op )
         {
         case op_callsubr:
         case op_div:
         case op_callothersubr:
         case op_pop:
         case op_return:
           break;
 
         default:
           if ( top - decoder->stack != num_args )
             FT_TRACE0(( "t1_decoder_parse_charstrings:"
                         " too much operands on the stack"
                         " (seen %d, expected %d)\n",
                         top - decoder->stack, num_args ));
             break;
         }
 
 #endif  
 
         top -= num_args;
 
         switch ( op )
         {
         case op_endchar:
           FT_TRACE4(( " endchar\n" ));
 
           t1_builder_close_contour( builder );
 
            
           if ( hinter )
           {
             if ( hinter->close( hinter->hints,
                                 (FT_UInt)builder->current->n_points ) )
               goto Syntax_Error;
 
              
             error = hinter->apply( hinter->hints,
                                    builder->current,
                                    (PSH_Globals)builder->hints_globals,
                                    decoder->hint_mode );
             if ( error )
               goto Fail;
           }
 
            
           FT_GlyphLoader_Add( builder->loader );
 
            
 
 #ifdef FT_DEBUG_LEVEL_TRACE
 
           if ( decoder->len_buildchar > 0 )
           {
             FT_UInt  i;
 
 
             FT_TRACE4(( "BuildCharArray = [ " ));
 
             for ( i = 0; i < decoder->len_buildchar; i++ )
               FT_TRACE4(( "%d ", decoder->buildchar[i] ));
 
             FT_TRACE4(( "]\n" ));
           }
 
 #endif  
 
           FT_TRACE4(( "\n" ));
 
            
           return FT_Err_Ok;
 
         case op_hsbw:
           FT_TRACE4(( " hsbw" ));
 
           builder->parse_state = T1_Parse_Have_Width;
 
           builder->left_bearing.x += top[0];
           builder->advance.x       = top[1];
           builder->advance.y       = 0;
 
           orig_x = x = builder->pos_x + top[0];
           orig_y = y = builder->pos_y;
 
           FT_UNUSED( orig_y );
 
            
            
            
           if ( builder->metrics_only )
             return FT_Err_Ok;
 
           break;
 
         case op_seac:
           return t1operator_seac( decoder,
                                   top[0],
                                   top[1],
                                   top[2],
                                   Fix2Int( top[3] ),
                                   Fix2Int( top[4] ) );
 
         case op_sbw:
           FT_TRACE4(( " sbw" ));
 
           builder->parse_state = T1_Parse_Have_Width;
 
           builder->left_bearing.x += top[0];
           builder->left_bearing.y += top[1];
           builder->advance.x       = top[2];
           builder->advance.y       = top[3];
 
           x = builder->pos_x + top[0];
           y = builder->pos_y + top[1];
 
            
            
            
           if ( builder->metrics_only )
             return FT_Err_Ok;
 
           break;
 
         case op_closepath:
           FT_TRACE4(( " closepath" ));
 
            
           if ( builder->parse_state == T1_Parse_Have_Path   ||
                builder->parse_state == T1_Parse_Have_Moveto )
             t1_builder_close_contour( builder );
 
           builder->parse_state = T1_Parse_Have_Width;
           break;
 
         case op_hlineto:
           FT_TRACE4(( " hlineto" ));
 
           if ( FT_SET_ERROR( t1_builder_start_point( builder, x, y ) ) )
             goto Fail;
 
           x += top[0];
           goto Add_Line;
 
         case op_hmoveto:
           FT_TRACE4(( " hmoveto" ));
 
           x += top[0];
           if ( !decoder->flex_state )
           {
             if ( builder->parse_state == T1_Parse_Start )
               goto Syntax_Error;
             builder->parse_state = T1_Parse_Have_Moveto;
           }
           break;
 
         case op_hvcurveto:
           FT_TRACE4(( " hvcurveto" ));
 
           if ( FT_SET_ERROR( t1_builder_start_point( builder, x, y ) ) ||
                FT_SET_ERROR( t1_builder_check_points( builder, 3 ) )   )
             goto Fail;
 
           x += top[0];
           t1_builder_add_point( builder, x, y, 0 );
           x += top[1];
           y += top[2];
           t1_builder_add_point( builder, x, y, 0 );
           y += top[3];
           t1_builder_add_point( builder, x, y, 1 );
           break;
 
         case op_rlineto:
           FT_TRACE4(( " rlineto" ));
 
           if ( FT_SET_ERROR( t1_builder_start_point( builder, x, y ) ) )
             goto Fail;
 
           x += top[0];
           y += top[1];
 
         Add_Line:
           if ( FT_SET_ERROR( t1_builder_add_point1( builder, x, y ) ) )
             goto Fail;
           break;
 
         case op_rmoveto:
           FT_TRACE4(( " rmoveto" ));
 
           x += top[0];
           y += top[1];
           if ( !decoder->flex_state )
           {
             if ( builder->parse_state == T1_Parse_Start )
               goto Syntax_Error;
             builder->parse_state = T1_Parse_Have_Moveto;
           }
           break;
 
         case op_rrcurveto:
           FT_TRACE4(( " rrcurveto" ));
 
           if ( FT_SET_ERROR( t1_builder_start_point( builder, x, y ) ) ||
                FT_SET_ERROR( t1_builder_check_points( builder, 3 ) )   )
             goto Fail;
 
           x += top[0];
           y += top[1];
           t1_builder_add_point( builder, x, y, 0 );
 
           x += top[2];
           y += top[3];
           t1_builder_add_point( builder, x, y, 0 );
 
           x += top[4];
           y += top[5];
           t1_builder_add_point( builder, x, y, 1 );
           break;
 
         case op_vhcurveto:
           FT_TRACE4(( " vhcurveto" ));
 
           if ( FT_SET_ERROR( t1_builder_start_point( builder, x, y ) ) ||
                FT_SET_ERROR( t1_builder_check_points( builder, 3 ) )   )
             goto Fail;
 
           y += top[0];
           t1_builder_add_point( builder, x, y, 0 );
           x += top[1];
           y += top[2];
           t1_builder_add_point( builder, x, y, 0 );
           x += top[3];
           t1_builder_add_point( builder, x, y, 1 );
           break;
 
         case op_vlineto:
           FT_TRACE4(( " vlineto" ));
 
           if ( FT_SET_ERROR( t1_builder_start_point( builder, x, y ) ) )
             goto Fail;
 
           y += top[0];
           goto Add_Line;
 
         case op_vmoveto:
           FT_TRACE4(( " vmoveto" ));
 
           y += top[0];
           if ( !decoder->flex_state )
           {
             if ( builder->parse_state == T1_Parse_Start )
               goto Syntax_Error;
             builder->parse_state = T1_Parse_Have_Moveto;
           }
           break;
 
         case op_div:
           FT_TRACE4(( " div" ));
 
            
            
            
           *top = FT_DivFix( top[0], top[1] );
           top++;
 
           large_int = FALSE;
           break;
 
         case op_callsubr:
           {
             FT_Int  idx;
 
 
             FT_TRACE4(( " callsubr" ));
 
             idx = Fix2Int( top[0] );
 
             if ( decoder->subrs_hash )
             {
               size_t*  val = ft_hash_num_lookup( idx,
                                                  decoder->subrs_hash );
 
 
               if ( val )
                 idx = *val;
               else
                 idx = -1;
             }
 
             if ( idx < 0 || idx >= decoder->num_subrs )
             {
               FT_ERROR(( "t1_decoder_parse_charstrings:"
                          " invalid subrs index\n" ));
               goto Syntax_Error;
             }
 
             if ( zone - decoder->zones >= T1_MAX_SUBRS_CALLS )
             {
               FT_ERROR(( "t1_decoder_parse_charstrings:"
                          " too many nested subrs\n" ));
               goto Syntax_Error;
             }
 
             zone->cursor = ip;   
 
             zone++;
 
              
              
              
             zone->base = decoder->subrs[idx];
 
             if ( decoder->subrs_len )
               zone->limit = zone->base + decoder->subrs_len[idx];
             else
             {
                
                
               zone->base  += ( decoder->lenIV >= 0 ? decoder->lenIV : 0 );
               zone->limit  = decoder->subrs[idx + 1];
             }
 
             zone->cursor = zone->base;
 
             if ( !zone->base )
             {
               FT_ERROR(( "t1_decoder_parse_charstrings:"
                          " invoking empty subrs\n" ));
               goto Syntax_Error;
             }
 
             decoder->zone = zone;
             ip            = zone->base;
             limit         = zone->limit;
             break;
           }
 
         case op_pop:
           FT_TRACE4(( " pop" ));
 
           if ( known_othersubr_result_cnt > 0 )
           {
             known_othersubr_result_cnt--;
              
             break;
           }
 
           if ( unknown_othersubr_result_cnt == 0 )
           {
             FT_ERROR(( "t1_decoder_parse_charstrings:"
                        " no more operands for othersubr\n" ));
             goto Syntax_Error;
           }
 
           unknown_othersubr_result_cnt--;
           top++;    
           break;
 
         case op_return:
           FT_TRACE4(( " return" ));
 
           if ( zone <= decoder->zones )
           {
             FT_ERROR(( "t1_decoder_parse_charstrings:"
                        " unexpected return\n" ));
             goto Syntax_Error;
           }
 
           zone--;
           ip            = zone->cursor;
           limit         = zone->limit;
           decoder->zone = zone;
           break;
 
         case op_dotsection:
           FT_TRACE4(( " dotsection" ));
 
           break;
 
         case op_hstem:
           FT_TRACE4(( " hstem" ));
 
            
           if ( hinter )
           {
              
             hinter->stem( hinter->hints, 1, top );
           }
           break;
 
         case op_hstem3:
           FT_TRACE4(( " hstem3" ));
 
            
           if ( hinter )
             hinter->stem3( hinter->hints, 1, top );
           break;
 
         case op_vstem:
           FT_TRACE4(( " vstem" ));
 
            
           if ( hinter )
           {
             top[0] += orig_x;
             hinter->stem( hinter->hints, 0, top );
           }
           break;
 
         case op_vstem3:
           FT_TRACE4(( " vstem3" ));
 
            
           if ( hinter )
           {
             FT_Pos  dx = orig_x;
 
 
             top[0] += dx;
             top[2] += dx;
             top[4] += dx;
             hinter->stem3( hinter->hints, 0, top );
           }
           break;
 
         case op_setcurrentpoint:
           FT_TRACE4(( " setcurrentpoint" ));
 
            
            
            
            
 
            
            
 
            
            
            
            
 #if 0
 
           if ( decoder->flex_state != 1 )
           {
             FT_ERROR(( "t1_decoder_parse_charstrings:"
                        " unexpected `setcurrentpoint'\n" ));
             goto Syntax_Error;
           }
           else
             ...
 #endif
 
           x = top[0];
           y = top[1];
           decoder->flex_state = 0;
           break;
 
         case op_unknown15:
           FT_TRACE4(( " opcode_15" ));
            
           break;
 
         default:
           FT_ERROR(( "t1_decoder_parse_charstrings:"
                      " unhandled opcode %d\n", op ));
           goto Syntax_Error;
         }
 
          
          
          
          
 
         decoder->top = top;
 
 #ifdef FT_DEBUG_LEVEL_TRACE
         FT_TRACE4(( "\n" ));
         bol = TRUE;
 #endif
 
       }  
 
     }  
 
     FT_TRACE4(( "..end..\n\n" ));
 
   Fail:
     return error;
 
   Syntax_Error:
     return FT_THROW( Syntax_Error );
 
   Stack_Underflow:
     return FT_THROW( Stack_Underflow );
   }