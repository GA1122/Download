  GetShortIns( TT_ExecContext  exc )
  {
     
    exc->IP += 2;
    return (FT_Short)( ( exc->code[exc->IP - 2] << 8 ) +
                         exc->code[exc->IP - 1]      );
  }
