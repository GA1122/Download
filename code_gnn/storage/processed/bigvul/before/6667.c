  t1_load_keyword( T1_Face         face,
                   T1_Loader       loader,
                   const T1_Field  field )
  {
    FT_Error  error;
    void*     dummy_object;
    void**    objects;
    FT_UInt   max_objects;
    PS_Blend  blend = face->blend;


    if ( blend && blend->num_designs == 0 )
      blend = NULL;

     
    if ( field->type == T1_FIELD_TYPE_CALLBACK )
    {
      field->reader( (FT_Face)face, loader );
      error = loader->parser.root.error;
      goto Exit;
    }

     
     
    switch ( field->location )
    {
    case T1_FIELD_LOCATION_FONT_INFO:
      dummy_object = &face->type1.font_info;
      objects      = &dummy_object;
      max_objects  = 0;

      if ( blend )
      {
        objects     = (void**)blend->font_infos;
        max_objects = blend->num_designs;
      }
      break;

    case T1_FIELD_LOCATION_FONT_EXTRA:
      dummy_object = &face->type1.font_extra;
      objects      = &dummy_object;
      max_objects  = 0;
      break;

    case T1_FIELD_LOCATION_PRIVATE:
      dummy_object = &face->type1.private_dict;
      objects      = &dummy_object;
      max_objects  = 0;

      if ( blend )
      {
        objects     = (void**)blend->privates;
        max_objects = blend->num_designs;
      }
      break;

    case T1_FIELD_LOCATION_BBOX:
      dummy_object = &face->type1.font_bbox;
      objects      = &dummy_object;
      max_objects  = 0;

      if ( blend )
      {
        objects     = (void**)blend->bboxes;
        max_objects = blend->num_designs;
      }
      break;

    case T1_FIELD_LOCATION_LOADER:
      dummy_object = loader;
      objects      = &dummy_object;
      max_objects  = 0;
      break;

    case T1_FIELD_LOCATION_FACE:
      dummy_object = face;
      objects      = &dummy_object;
      max_objects  = 0;
      break;

#ifndef T1_CONFIG_OPTION_NO_MM_SUPPORT
    case T1_FIELD_LOCATION_BLEND:
      dummy_object = face->blend;
      objects      = &dummy_object;
      max_objects  = 0;
      break;
#endif

    default:
      dummy_object = &face->type1;
      objects      = &dummy_object;
      max_objects  = 0;
    }

    if ( *objects )
    {
      if ( field->type == T1_FIELD_TYPE_INTEGER_ARRAY ||
           field->type == T1_FIELD_TYPE_FIXED_ARRAY   )
        error = T1_Load_Field_Table( &loader->parser, field,
                                     objects, max_objects, 0 );
      else
        error = T1_Load_Field( &loader->parser, field,
                               objects, max_objects, 0 );
    }
    else
    {
      FT_TRACE1(( "t1_load_keyword: ignoring keyword `%s'"
                  " which is not valid at this point\n"
                  "                 (probably due to missing keywords)\n",
                 field->ident ));
      error = FT_Err_Ok;
    }

  Exit:
    return error;
  }
