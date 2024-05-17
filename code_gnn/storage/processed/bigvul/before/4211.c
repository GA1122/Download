  cff_charset_free_cids( CFF_Charset  charset,
                         FT_Memory    memory )
  {
    FT_FREE( charset->cids );
    charset->max_cid = 0;
  }
