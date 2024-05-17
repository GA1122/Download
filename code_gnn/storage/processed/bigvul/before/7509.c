  tt_check_trickyness_family( FT_String*  name )
  {

#define TRICK_NAMES_MAX_CHARACTERS  19
#define TRICK_NAMES_COUNT            9

    static const char trick_names[TRICK_NAMES_COUNT]
                                 [TRICK_NAMES_MAX_CHARACTERS + 1] =
    {
      "DFKaiSho-SB",         
      "DFKaiShu",
      "DFKai-SB",            
      "HuaTianKaiTi?",       
      "HuaTianSongTi?",      
      "Ming(for ISO10646)",  
      "MingLiU",             
      "PMingLiU",            
      "MingLi43",            
    };

    int  nn;


    for ( nn = 0; nn < TRICK_NAMES_COUNT; nn++ )
      if ( ft_strstr( name, trick_names[nn] ) )
        return TRUE;

    return FALSE;
  }
