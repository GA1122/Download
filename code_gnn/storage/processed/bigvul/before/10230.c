  FT_Done_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_ListNode  node;


    error = FT_Err_Invalid_Face_Handle;
    if ( face && face->driver )
    {
      driver = face->driver;
      memory = driver->root.memory;

       
      node = FT_List_Find( &driver->faces_list, face );
      if ( node )
      {
         
        FT_List_Remove( &driver->faces_list, node );
        FT_FREE( node );

         
        destroy_face( memory, face, driver );
        error = FT_Err_Ok;
      }
    }
    return error;
  }
