  cid_load_keyword( CID_Face        face,
                    CID_Loader*     loader,
                    const T1_Field  keyword )
  {
    FT_Error      error;
    CID_Parser*   parser = &loader->parser;
    FT_Byte*      object;
    void*         dummy_object;
    CID_FaceInfo  cid = &face->cid;


     
    if ( keyword->type == T1_FIELD_TYPE_CALLBACK )
    {
      keyword->reader( (FT_Face)face, parser );
      error = parser->root.error;
      goto Exit;
    }

     
    switch ( keyword->location )
    {
    case T1_FIELD_LOCATION_CID_INFO:
      object = (FT_Byte*)cid;
      break;

    case T1_FIELD_LOCATION_FONT_INFO:
      object = (FT_Byte*)&cid->font_info;
      break;

    case T1_FIELD_LOCATION_FONT_EXTRA:
      object = (FT_Byte*)&face->font_extra;
      break;

    case T1_FIELD_LOCATION_BBOX:
      object = (FT_Byte*)&cid->font_bbox;
      break;

    default:
      {
        CID_FaceDict  dict;


        if ( parser->num_dict < 0 || parser->num_dict >= cid->num_dicts )
        {
          FT_ERROR(( "cid_load_keyword: invalid use of `%s'\n",
                     keyword->ident ));
          error = FT_THROW( Syntax_Error );
          goto Exit;
        }

        dict = cid->font_dicts + parser->num_dict;
        switch ( keyword->location )
        {
        case T1_FIELD_LOCATION_PRIVATE:
          object = (FT_Byte*)&dict->private_dict;
          break;

        default:
          object = (FT_Byte*)dict;
        }
      }
    }

    dummy_object = object;

     
    if ( keyword->type == T1_FIELD_TYPE_INTEGER_ARRAY ||
         keyword->type == T1_FIELD_TYPE_FIXED_ARRAY   )
      error = cid_parser_load_field_table( &loader->parser, keyword,
                                           &dummy_object );
    else
      error = cid_parser_load_field( &loader->parser,
                                     keyword, &dummy_object );
  Exit:
    return error;
  }
