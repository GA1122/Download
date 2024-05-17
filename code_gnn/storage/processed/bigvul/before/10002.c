  render_state_set_files( RenderState  state,
                          char**       files )
  {
    FontFace  faces     = NULL;
    int       num_faces = 0;
    int       max_faces = 0;


    state->files = files;
    for ( ; files[0] != NULL; files++ )
    {
      FT_Face   face;
      FT_Error  error = FT_New_Face( state->library, files[0], -1, &face );
      int       count;


      if ( error )
      {
        fprintf( stderr,
                 "ftdiff: could not open font file `%s'\n", files[0] );
        continue;
      }

      for ( count = 0; count < (int)face->num_faces; count++ )
      {
        if ( num_faces >= max_faces )
        {
          max_faces += ( max_faces >> 1 ) + 8;
          faces = (FontFace)realloc( faces,
                                     max_faces * sizeof ( faces[0] ) );
          if ( faces == NULL )
            panic("ftdiff: not enough memory\n");
        }

        faces[num_faces].filepath = files[0];
        faces[num_faces].index    = count;
        num_faces++;
      }

      FT_Done_Face( face );
    }

    state->faces     = faces;
    state->num_faces = num_faces;

    if ( num_faces == 0 )
    {
      fprintf( stderr, "ftdiff: no input font files!\n" );
      usage();
    }

    state->face_index = 0;
  }
