  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
     
     
     
    volatile ft_jmp_buf* jump_buffer = &valid->jump_buffer;


    valid->error = error;

     
     
    ft_longjmp( *(ft_jmp_buf*) jump_buffer, 1 );
  }
