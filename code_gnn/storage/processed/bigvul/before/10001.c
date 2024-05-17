  render_state_set_file( RenderState  state,
                         int          idx )
  {
    const char*  filepath;


    if ( idx < 0 )
      idx = state->num_faces - 1;
    else if ( idx >= state->num_faces )
      idx = 0;

    if ( idx >= state->num_faces )
      return -2;

    state->face_index = idx;
    filepath = state->faces[idx].filepath;

    if ( state->face )
    {
      FT_Done_Face( state->face );
      state->face         = NULL;
      state->size         = NULL;
      state->need_rescale = 1;
    }

    if ( filepath != NULL && filepath[0] != 0 )
    {
      FT_Error  error;


      error = FT_New_Face( state->library, filepath,
                           state->faces[idx].index, &state->face );
      if ( error )
        return -1;

      {
        unsigned int  len = strlen( filepath );
        char*         p;


        if ( len + 1 > sizeof ( state->filepath0 ) )
        {
          state->filepath = (const char*)malloc( len + 1 );
          if ( state->filepath == NULL )
          {
            state->filepath = state->filepath0;
            return -1;
          }
        }
        memcpy( (char*)state->filepath, filepath, len + 1 );
        p = strrchr( state->filepath, '\\' );
        if ( p == NULL )
          p = strrchr( state->filepath, '/' );

        state->filename = p ? p + 1 : state->filepath;
      }

      state->size         = state->face->size;
      state->need_rescale = 1;
    }

    return 0;
  }
