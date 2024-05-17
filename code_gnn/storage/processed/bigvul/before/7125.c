  T42_Size_Done( FT_Size  t42size )              
  {
    T42_Size     size    = (T42_Size)t42size;
    FT_Face      face    = t42size->face;
    T42_Face     t42face = (T42_Face)face;
    FT_ListNode  node;


    node = FT_List_Find( &t42face->ttf_face->sizes_list, size->ttsize );
    if ( node )
    {
      FT_Done_Size( size->ttsize );
      size->ttsize = NULL;
    }
  }
