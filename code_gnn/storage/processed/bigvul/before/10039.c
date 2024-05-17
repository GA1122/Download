  parse_design_coords( char  *s )
  {
    for ( requested_cnt = 0; requested_cnt < T1_MAX_MM_AXIS && *s;
          requested_cnt++ )
    {
      requested_pos[requested_cnt] = (FT_Fixed)( strtod( s, &s ) * 65536.0 );

      while ( *s==' ' )
        ++s;
    }
  }
