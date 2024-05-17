  Ins_FDEF( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_ULong       n;
    TT_DefRecord*  rec;
    TT_DefRecord*  limit;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
     
    FT_Byte    opcode_pattern[9][12] = {
                  
                 {
                   0x4B,  
                   0x53,  
                   0x23,  
                   0x4B,  
                   0x51,  
                   0x5A,  
                   0x58,  
                   0x38,  
                   0x1B,  
                   0x21,  
                   0x21,  
                   0x59   
                 },
                  
                 {
                   0x4B,  
                   0x54,  
                   0x58,  
                   0x38,  
                   0x1B,  
                   0x21,  
                   0x21,  
                   0x59   
                 },
                  
                 {
                   0x20,  
                   0x20,  
                   0xB0,  
                          
                   0x60,  
                   0x46,  
                   0xB0,  
                          
                   0x23,  
                   0x42   
                 },
                  
                 {
                   0x45,  
                   0x23,  
                   0x46,  
                   0x60,  
                   0x20,  
                   0xB0   
                          
                 },
                  
                 {
                   0x20,  
                   0x64,  
                   0xB0,  
                          
                   0x60,  
                   0x66,  
                   0x23,  
                   0xB0   
                 },
                  
                 {
                   0x01,  
                   0xB0,  
                          
                   0x43,  
                   0x58   
                 },
                  
                 {
                   0x01,  
                   0x18,  
                   0xB0,  
                          
                   0x43,  
                   0x58   
                 },
                  
                 {
                   0x01,  
                   0x20,  
                   0xB0,  
                          
                   0x25,  
                 },
                  
                 {
                   0x06,  
                   0x7D,  
                 },
               };
    FT_UShort  opcode_patterns   = 9;
    FT_UShort  opcode_pointer[9] = {  0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FT_UShort  opcode_size[9]    = { 12, 8, 8, 6, 7, 4, 5, 4, 2 };
    FT_UShort  i;
#endif  


     
    if ( exc->curRange == tt_coderange_glyph )
    {
      exc->error = FT_THROW( DEF_In_Glyf_Bytecode );
      return;
    }

     
     

    rec   = exc->FDefs;
    limit = rec + exc->numFDefs;
    n     = (FT_ULong)args[0];

    for ( ; rec < limit; rec++ )
    {
      if ( rec->opc == n )
        break;
    }

    if ( rec == limit )
    {
       
      if ( exc->numFDefs >= exc->maxFDefs )
      {
        exc->error = FT_THROW( Too_Many_Function_Defs );
        return;
      }
      exc->numFDefs++;
    }

     
     
    if ( n > 0xFFFFU )
    {
      exc->error = FT_THROW( Too_Many_Function_Defs );
      return;
    }

    rec->range          = exc->curRange;
    rec->opc            = (FT_UInt16)n;
    rec->start          = exc->IP + 1;
    rec->active         = TRUE;
    rec->inline_delta   = FALSE;
    rec->sph_fdef_flags = 0x0000;

    if ( n > exc->maxFunc )
      exc->maxFunc = (FT_UInt16)n;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
     
     
    if ( n >= 64 && n <= 66 )
      rec->sph_fdef_flags |= SPH_FDEF_TYPEMAN_STROKES;
#endif

     
     

    while ( SkipCode( exc ) == SUCCESS )
    {

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY

      if ( SUBPIXEL_HINTING_INFINALITY )
      {
        for ( i = 0; i < opcode_patterns; i++ )
        {
          if ( opcode_pointer[i] < opcode_size[i]                  &&
               exc->opcode == opcode_pattern[i][opcode_pointer[i]] )
          {
            opcode_pointer[i] += 1;

            if ( opcode_pointer[i] == opcode_size[i] )
            {
              FT_TRACE6(( "sph: Function %d, opcode ptrn: %d, %s %s\n",
                          i, n,
                          exc->face->root.family_name,
                          exc->face->root.style_name ));

              switch ( i )
              {
              case 0:
                rec->sph_fdef_flags             |= SPH_FDEF_INLINE_DELTA_1;
                exc->face->sph_found_func_flags |= SPH_FDEF_INLINE_DELTA_1;
                break;

              case 1:
                rec->sph_fdef_flags             |= SPH_FDEF_INLINE_DELTA_2;
                exc->face->sph_found_func_flags |= SPH_FDEF_INLINE_DELTA_2;
                break;

              case 2:
                switch ( n )
                {
                   
                case 58:
                  rec->sph_fdef_flags             |= SPH_FDEF_DIAGONAL_STROKE;
                  exc->face->sph_found_func_flags |= SPH_FDEF_DIAGONAL_STROKE;
                }
                break;

              case 3:
                switch ( n )
                {
                case 0:
                  rec->sph_fdef_flags             |= SPH_FDEF_VACUFORM_ROUND_1;
                  exc->face->sph_found_func_flags |= SPH_FDEF_VACUFORM_ROUND_1;
                }
                break;

              case 4:
                 
                rec->sph_fdef_flags             |= SPH_FDEF_TTFAUTOHINT_1;
                exc->face->sph_found_func_flags |= SPH_FDEF_TTFAUTOHINT_1;
                break;

              case 5:
                switch ( n )
                {
                case 0:
                case 1:
                case 2:
                case 4:
                case 7:
                case 8:
                  rec->sph_fdef_flags             |= SPH_FDEF_SPACING_1;
                  exc->face->sph_found_func_flags |= SPH_FDEF_SPACING_1;
                }
                break;

              case 6:
                switch ( n )
                {
                case 0:
                case 1:
                case 2:
                case 4:
                case 7:
                case 8:
                  rec->sph_fdef_flags             |= SPH_FDEF_SPACING_2;
                  exc->face->sph_found_func_flags |= SPH_FDEF_SPACING_2;
                }
                break;

               case 7:
                 rec->sph_fdef_flags             |= SPH_FDEF_TYPEMAN_DIAGENDCTRL;
                 exc->face->sph_found_func_flags |= SPH_FDEF_TYPEMAN_DIAGENDCTRL;
                 break;

               case 8:
#if 0
                 rec->sph_fdef_flags             |= SPH_FDEF_TYPEMAN_DIAGENDCTRL;
                 exc->face->sph_found_func_flags |= SPH_FDEF_TYPEMAN_DIAGENDCTRL;
#endif
                 break;
              }
              opcode_pointer[i] = 0;
            }
          }

          else
            opcode_pointer[i] = 0;
        }

         
        exc->face->sph_compatibility_mode =
          ( ( exc->face->sph_found_func_flags & SPH_FDEF_INLINE_DELTA_1 ) |
            ( exc->face->sph_found_func_flags & SPH_FDEF_INLINE_DELTA_2 ) );
      }

#endif  

      switch ( exc->opcode )
      {
      case 0x89:     
      case 0x2C:     
        exc->error = FT_THROW( Nested_DEFS );
        return;

      case 0x2D:    
        rec->end = exc->IP;
        return;
      }
    }
  }
