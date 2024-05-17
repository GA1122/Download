  compare_offsets( const void*  a,
                   const void*  b )
  {
    WOFF_Table  table1 = *(WOFF_Table*)a;
    WOFF_Table  table2 = *(WOFF_Table*)b;

    FT_ULong  offset1 = table1->Offset;
    FT_ULong  offset2 = table2->Offset;


    if ( offset1 > offset2 )
      return 1;
    else if ( offset1 < offset2 )
      return -1;
    else
      return 0;
  }
