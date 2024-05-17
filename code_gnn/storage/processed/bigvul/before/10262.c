  FT_Render_Glyph_Internal( FT_Library      library,
                            FT_GlyphSlot    slot,
                            FT_Render_Mode  render_mode )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Renderer  renderer;


     
    switch ( slot->format )
    {
    case FT_GLYPH_FORMAT_BITMAP:    
      break;

    default:
      {
        FT_ListNode  node   = 0;
        FT_Bool      update = 0;


         
        if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          renderer = library->cur_renderer;
          node     = library->renderers.head;
        }
        else
          renderer = FT_Lookup_Renderer( library, slot->format, &node );

        error = FT_Err_Unimplemented_Feature;
        while ( renderer )
        {
          error = renderer->render( renderer, slot, render_mode, NULL );
          if ( !error                                               ||
               FT_ERROR_BASE( error ) != FT_Err_Cannot_Render_Glyph )
            break;

           
           
           

           
           
          renderer = FT_Lookup_Renderer( library, slot->format, &node );
          update   = 1;
        }

         
         
        if ( !error && update && renderer )
          FT_Set_Renderer( library, renderer, 0, 0 );
      }
    }

    return error;
  }
