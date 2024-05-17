  TT_RunIns( TT_ExecContext  exc )
  {
    FT_ULong   ins_counter = 0;   
    FT_ULong   num_twilight_points;
    FT_UShort  i;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    FT_Byte    opcode_pattern[1][2] = {
                   
                  {
                    0x06,  
                    0x7D,  
                  },
                };
    FT_UShort  opcode_patterns   = 1;
    FT_UShort  opcode_pointer[1] = { 0 };
    FT_UShort  opcode_size[1]    = { 1 };
#endif  


#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    exc->iup_called = FALSE;
#endif  

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
     
    if ( SUBPIXEL_HINTING_MINIMAL          &&
         exc->subpixel_hinting_lean        &&
         !FT_IS_TRICKY( &exc->face->root ) )
      exc->backward_compatibility = !( exc->GS.instruct_control & 4 );
    else
      exc->backward_compatibility = FALSE;

    exc->iupx_called = FALSE;
    exc->iupy_called = FALSE;
#endif

     
     
    num_twilight_points = FT_MAX( 30,
                                  2 * ( exc->pts.n_points + exc->cvtSize ) );
    if ( exc->twilight.n_points > num_twilight_points )
    {
      if ( num_twilight_points > 0xFFFFU )
        num_twilight_points = 0xFFFFU;

      FT_TRACE5(( "TT_RunIns: Resetting number of twilight points\n"
                  "           from %d to the more reasonable value %d\n",
                  exc->twilight.n_points,
                  num_twilight_points ));
      exc->twilight.n_points = (FT_UShort)num_twilight_points;
    }

     
     
     
     
     
     
     
     
     
     
    exc->loopcall_counter = 0;
    exc->neg_jump_counter = 0;

     
    if ( exc->pts.n_points )
      exc->loopcall_counter_max = FT_MAX( 50,
                                          10 * exc->pts.n_points ) +
                                  FT_MAX( 50,
                                          exc->cvtSize / 10 );
    else
      exc->loopcall_counter_max = 300 + 8 * exc->cvtSize;

     
     
    if ( exc->loopcall_counter_max >
         100 * (FT_ULong)exc->face->root.num_glyphs )
      exc->loopcall_counter_max = 100 * (FT_ULong)exc->face->root.num_glyphs;

    FT_TRACE5(( "TT_RunIns: Limiting total number of loops in LOOPCALL"
                " to %d\n", exc->loopcall_counter_max ));

    exc->neg_jump_counter_max = exc->loopcall_counter_max;
    FT_TRACE5(( "TT_RunIns: Limiting total number of backward jumps"
                " to %d\n", exc->neg_jump_counter_max ));

     
    exc->tt_metrics.ratio = 0;
    if ( exc->metrics.x_ppem != exc->metrics.y_ppem )
    {
       
      exc->func_cur_ppem  = Current_Ppem_Stretched;
      exc->func_read_cvt  = Read_CVT_Stretched;
      exc->func_write_cvt = Write_CVT_Stretched;
      exc->func_move_cvt  = Move_CVT_Stretched;
    }
    else
    {
       
      exc->func_cur_ppem  = Current_Ppem;
      exc->func_read_cvt  = Read_CVT;
      exc->func_write_cvt = Write_CVT;
      exc->func_move_cvt  = Move_CVT;
    }

    Compute_Funcs( exc );
    Compute_Round( exc, (FT_Byte)exc->GS.round_state );

    do
    {
      exc->opcode = exc->code[exc->IP];

#ifdef FT_DEBUG_LEVEL_TRACE
      {
        FT_Long  cnt = FT_MIN( 8, exc->top );
        FT_Long  n;


         
         
        FT_TRACE6(( "  " ));
        FT_TRACE7(( "%06d ", exc->IP ));
        FT_TRACE6(( opcode_name[exc->opcode] + 2 ));
        FT_TRACE7(( "%*s", *opcode_name[exc->opcode] == 'A'
                              ? 2
                              : 12 - ( *opcode_name[exc->opcode] - '0' ),
                              "#" ));
        for ( n = 1; n <= cnt; n++ )
          FT_TRACE7(( " %d", exc->stack[exc->top - n] ));
        FT_TRACE6(( "\n" ));
      }
#endif  

      if ( ( exc->length = opcode_length[exc->opcode] ) < 0 )
      {
        if ( exc->IP + 1 >= exc->codeSize )
          goto LErrorCodeOverflow_;

        exc->length = 2 - exc->length * exc->code[exc->IP + 1];
      }

      if ( exc->IP + exc->length > exc->codeSize )
        goto LErrorCodeOverflow_;

       
      exc->args = exc->top - ( Pop_Push_Count[exc->opcode] >> 4 );

       
       
      if ( exc->args < 0 )
      {
        if ( exc->pedantic_hinting )
        {
          exc->error = FT_THROW( Too_Few_Arguments );
          goto LErrorLabel_;
        }

         
        for ( i = 0; i < Pop_Push_Count[exc->opcode] >> 4; i++ )
          exc->stack[i] = 0;
        exc->args = 0;
      }

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
      if ( exc->opcode == 0x91 )
      {
         
         

         
         
         
        if ( exc->face->blend )
          exc->new_top = exc->args + exc->face->blend->num_axis;
      }
      else
#endif
        exc->new_top = exc->args + ( Pop_Push_Count[exc->opcode] & 15 );

       
       
       
      if ( exc->new_top > exc->stackSize )
      {
        exc->error = FT_THROW( Stack_Overflow );
        goto LErrorLabel_;
      }

      exc->step_ins = TRUE;
      exc->error    = FT_Err_Ok;

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
              FT_TRACE6(( "sph: opcode ptrn: %d, %s %s\n",
                          i,
                          exc->face->root.family_name,
                          exc->face->root.style_name ));

              switch ( i )
              {
              case 0:
                break;
              }
              opcode_pointer[i] = 0;
            }
          }
          else
            opcode_pointer[i] = 0;
        }
      }

#endif  

      {
        FT_Long*  args   = exc->stack + exc->args;
        FT_Byte   opcode = exc->opcode;


        switch ( opcode )
        {
        case 0x00:   
        case 0x01:   
        case 0x02:   
        case 0x03:   
        case 0x04:   
        case 0x05:   
          Ins_SxyTCA( exc );
          break;

        case 0x06:   
        case 0x07:   
          Ins_SPVTL( exc, args );
          break;

        case 0x08:   
        case 0x09:   
          Ins_SFVTL( exc, args );
          break;

        case 0x0A:   
          Ins_SPVFS( exc, args );
          break;

        case 0x0B:   
          Ins_SFVFS( exc, args );
          break;

        case 0x0C:   
          Ins_GPV( exc, args );
          break;

        case 0x0D:   
          Ins_GFV( exc, args );
          break;

        case 0x0E:   
          Ins_SFVTPV( exc );
          break;

        case 0x0F:   
          Ins_ISECT( exc, args );
          break;

        case 0x10:   
          Ins_SRP0( exc, args );
          break;

        case 0x11:   
          Ins_SRP1( exc, args );
          break;

        case 0x12:   
          Ins_SRP2( exc, args );
          break;

        case 0x13:   
          Ins_SZP0( exc, args );
          break;

        case 0x14:   
          Ins_SZP1( exc, args );
          break;

        case 0x15:   
          Ins_SZP2( exc, args );
          break;

        case 0x16:   
          Ins_SZPS( exc, args );
          break;

        case 0x17:   
          Ins_SLOOP( exc, args );
          break;

        case 0x18:   
          Ins_RTG( exc );
          break;

        case 0x19:   
          Ins_RTHG( exc );
          break;

        case 0x1A:   
          Ins_SMD( exc, args );
          break;

        case 0x1B:   
          Ins_ELSE( exc );
          break;

        case 0x1C:   
          Ins_JMPR( exc, args );
          break;

        case 0x1D:   
          Ins_SCVTCI( exc, args );
          break;

        case 0x1E:   
          Ins_SSWCI( exc, args );
          break;

        case 0x1F:   
          Ins_SSW( exc, args );
          break;

        case 0x20:   
          Ins_DUP( args );
          break;

        case 0x21:   
          Ins_POP();
          break;

        case 0x22:   
          Ins_CLEAR( exc );
          break;

        case 0x23:   
          Ins_SWAP( args );
          break;

        case 0x24:   
          Ins_DEPTH( exc, args );
          break;

        case 0x25:   
          Ins_CINDEX( exc, args );
          break;

        case 0x26:   
          Ins_MINDEX( exc, args );
          break;

        case 0x27:   
          Ins_ALIGNPTS( exc, args );
          break;

        case 0x28:   
          Ins_UNKNOWN( exc );
          break;

        case 0x29:   
          Ins_UTP( exc, args );
          break;

        case 0x2A:   
          Ins_LOOPCALL( exc, args );
          break;

        case 0x2B:   
          Ins_CALL( exc, args );
          break;

        case 0x2C:   
          Ins_FDEF( exc, args );
          break;

        case 0x2D:   
          Ins_ENDF( exc );
          break;

        case 0x2E:   
        case 0x2F:   
          Ins_MDAP( exc, args );
          break;

        case 0x30:   
        case 0x31:   
          Ins_IUP( exc );
          break;

        case 0x32:   
        case 0x33:   
          Ins_SHP( exc );
          break;

        case 0x34:   
        case 0x35:   
          Ins_SHC( exc, args );
          break;

        case 0x36:   
        case 0x37:   
          Ins_SHZ( exc, args );
          break;

        case 0x38:   
          Ins_SHPIX( exc, args );
          break;

        case 0x39:   
          Ins_IP( exc );
          break;

        case 0x3A:   
        case 0x3B:   
          Ins_MSIRP( exc, args );
          break;

        case 0x3C:   
          Ins_ALIGNRP( exc );
          break;

        case 0x3D:   
          Ins_RTDG( exc );
          break;

        case 0x3E:   
        case 0x3F:   
          Ins_MIAP( exc, args );
          break;

        case 0x40:   
          Ins_NPUSHB( exc, args );
          break;

        case 0x41:   
          Ins_NPUSHW( exc, args );
          break;

        case 0x42:   
          Ins_WS( exc, args );
          break;

        case 0x43:   
          Ins_RS( exc, args );
          break;

        case 0x44:   
          Ins_WCVTP( exc, args );
          break;

        case 0x45:   
          Ins_RCVT( exc, args );
          break;

        case 0x46:   
        case 0x47:   
          Ins_GC( exc, args );
          break;

        case 0x48:   
          Ins_SCFS( exc, args );
          break;

        case 0x49:   
        case 0x4A:   
          Ins_MD( exc, args );
          break;

        case 0x4B:   
          Ins_MPPEM( exc, args );
          break;

        case 0x4C:   
          Ins_MPS( exc, args );
          break;

        case 0x4D:   
          Ins_FLIPON( exc );
          break;

        case 0x4E:   
          Ins_FLIPOFF( exc );
          break;

        case 0x4F:   
          Ins_DEBUG( exc );
          break;

        case 0x50:   
          Ins_LT( args );
          break;

        case 0x51:   
          Ins_LTEQ( args );
          break;

        case 0x52:   
          Ins_GT( args );
          break;

        case 0x53:   
          Ins_GTEQ( args );
          break;

        case 0x54:   
          Ins_EQ( args );
          break;

        case 0x55:   
          Ins_NEQ( args );
          break;

        case 0x56:   
          Ins_ODD( exc, args );
          break;

        case 0x57:   
          Ins_EVEN( exc, args );
          break;

        case 0x58:   
          Ins_IF( exc, args );
          break;

        case 0x59:   
          Ins_EIF();
          break;

        case 0x5A:   
          Ins_AND( args );
          break;

        case 0x5B:   
          Ins_OR( args );
          break;

        case 0x5C:   
          Ins_NOT( args );
          break;

        case 0x5D:   
          Ins_DELTAP( exc, args );
          break;

        case 0x5E:   
          Ins_SDB( exc, args );
          break;

        case 0x5F:   
          Ins_SDS( exc, args );
          break;

        case 0x60:   
          Ins_ADD( args );
          break;

        case 0x61:   
          Ins_SUB( args );
          break;

        case 0x62:   
          Ins_DIV( exc, args );
          break;

        case 0x63:   
          Ins_MUL( args );
          break;

        case 0x64:   
          Ins_ABS( args );
          break;

        case 0x65:   
          Ins_NEG( args );
          break;

        case 0x66:   
          Ins_FLOOR( args );
          break;

        case 0x67:   
          Ins_CEILING( args );
          break;

        case 0x68:   
        case 0x69:   
        case 0x6A:   
        case 0x6B:   
          Ins_ROUND( exc, args );
          break;

        case 0x6C:   
        case 0x6D:   
        case 0x6E:   
        case 0x6F:   
          Ins_NROUND( exc, args );
          break;

        case 0x70:   
          Ins_WCVTF( exc, args );
          break;

        case 0x71:   
        case 0x72:   
          Ins_DELTAP( exc, args );
          break;

        case 0x73:   
        case 0x74:   
        case 0x75:   
          Ins_DELTAC( exc, args );
          break;

        case 0x76:   
          Ins_SROUND( exc, args );
          break;

        case 0x77:   
          Ins_S45ROUND( exc, args );
          break;

        case 0x78:   
          Ins_JROT( exc, args );
          break;

        case 0x79:   
          Ins_JROF( exc, args );
          break;

        case 0x7A:   
          Ins_ROFF( exc );
          break;

        case 0x7B:   
          Ins_UNKNOWN( exc );
          break;

        case 0x7C:   
          Ins_RUTG( exc );
          break;

        case 0x7D:   
          Ins_RDTG( exc );
          break;

        case 0x7E:   
          Ins_SANGW();
          break;

        case 0x7F:   
          Ins_AA();
          break;

        case 0x80:   
          Ins_FLIPPT( exc );
          break;

        case 0x81:   
          Ins_FLIPRGON( exc, args );
          break;

        case 0x82:   
          Ins_FLIPRGOFF( exc, args );
          break;

        case 0x83:   
        case 0x84:   
          Ins_UNKNOWN( exc );
          break;

        case 0x85:   
          Ins_SCANCTRL( exc, args );
          break;

        case 0x86:   
        case 0x87:   
          Ins_SDPVTL( exc, args );
          break;

        case 0x88:   
          Ins_GETINFO( exc, args );
          break;

        case 0x89:   
          Ins_IDEF( exc, args );
          break;

        case 0x8A:   
          Ins_ROLL( args );
          break;

        case 0x8B:   
          Ins_MAX( args );
          break;

        case 0x8C:   
          Ins_MIN( args );
          break;

        case 0x8D:   
          Ins_SCANTYPE( exc, args );
          break;

        case 0x8E:   
          Ins_INSTCTRL( exc, args );
          break;

        case 0x8F:   
        case 0x90:   
          Ins_UNKNOWN( exc );
          break;

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
        case 0x91:
           
           
           
          if ( exc->face->blend )
            Ins_GETVARIATION( exc, args );
          else
            Ins_UNKNOWN( exc );
          break;

        case 0x92:
           
           
           
          if ( exc->face->blend )
            Ins_GETDATA( args );
          else
            Ins_UNKNOWN( exc );
          break;
#endif

        default:
          if ( opcode >= 0xE0 )
            Ins_MIRP( exc, args );
          else if ( opcode >= 0xC0 )
            Ins_MDRP( exc, args );
          else if ( opcode >= 0xB8 )
            Ins_PUSHW( exc, args );
          else if ( opcode >= 0xB0 )
            Ins_PUSHB( exc, args );
          else
            Ins_UNKNOWN( exc );
        }
      }

      if ( exc->error )
      {
        switch ( exc->error )
        {
           
        case FT_ERR( Invalid_Opcode ):
          {
            TT_DefRecord*  def   = exc->IDefs;
            TT_DefRecord*  limit = def + exc->numIDefs;


            for ( ; def < limit; def++ )
            {
              if ( def->active && exc->opcode == (FT_Byte)def->opc )
              {
                TT_CallRec*  callrec;


                if ( exc->callTop >= exc->callSize )
                {
                  exc->error = FT_THROW( Invalid_Reference );
                  goto LErrorLabel_;
                }

                callrec = &exc->callStack[exc->callTop];

                callrec->Caller_Range = exc->curRange;
                callrec->Caller_IP    = exc->IP + 1;
                callrec->Cur_Count    = 1;
                callrec->Def          = def;

                if ( Ins_Goto_CodeRange( exc,
                                         def->range,
                                         def->start ) == FAILURE )
                  goto LErrorLabel_;

                goto LSuiteLabel_;
              }
            }
          }

          exc->error = FT_THROW( Invalid_Opcode );
          goto LErrorLabel_;

#if 0
          break;    
                    
                    
#endif

        default:
          goto LErrorLabel_;

#if 0
        break;
#endif
        }
      }

      exc->top = exc->new_top;

      if ( exc->step_ins )
        exc->IP += exc->length;

       
       
      if ( ++ins_counter > TT_CONFIG_OPTION_MAX_RUNNABLE_OPCODES )
        return FT_THROW( Execution_Too_Long );

    LSuiteLabel_:
      if ( exc->IP >= exc->codeSize )
      {
        if ( exc->callTop > 0 )
        {
          exc->error = FT_THROW( Code_Overflow );
          goto LErrorLabel_;
        }
        else
          goto LNo_Error_;
      }
    } while ( !exc->instruction_trap );

  LNo_Error_:
    FT_TRACE4(( "  %d instruction%s executed\n",
                ins_counter == 1 ? "" : "s",
                ins_counter ));
    return FT_Err_Ok;

  LErrorCodeOverflow_:
    exc->error = FT_THROW( Code_Overflow );

  LErrorLabel_:
    if ( exc->error && !exc->instruction_trap )
      FT_TRACE1(( "  The interpreter returned error 0x%x\n", exc->error ));

    return exc->error;
  }
