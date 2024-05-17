   Horizontal_Sweep_Drop( RAS_ARGS Short       y,
                                   FT_F26Dot6  x1,
                                   FT_F26Dot6  x2,
                                   PProfile    left,
                                   PProfile    right )
   {
     Long   e1, e2, pxl;
     PByte  bits;
     Byte   f1;
 
 
      
 
      
      
      
      
      
      
      
      
      
 
     e1  = CEILING( x1 );
     e2  = FLOOR  ( x2 );
     pxl = e1;
 
     if ( e1 > e2 )
     {
       Int  dropOutControl = left->flags & 7;
 
 
       if ( e1 == e2 + ras.precision )
       {
         switch ( dropOutControl )
         {
         case 0:  
           pxl = e2;
           break;
 
         case 4:  
           pxl = FLOOR( ( x1 + x2 - 1 ) / 2 + ras.precision_half );
           break;
 
         case 1:  
         case 5:  
            
 
            
           if ( left->next == right                &&
                left->height <= 0                  &&
                !( left->flags & Overshoot_Top   &&
                   x2 - x1 >= ras.precision_half ) )
             return;
 
            
           if ( right->next == left                 &&
                left->start == y                    &&
                !( left->flags & Overshoot_Bottom &&
                   x2 - x1 >= ras.precision_half  ) )
             return;
 
           if ( dropOutControl == 1 )
             pxl = e2;
           else
             pxl = FLOOR( ( x1 + x2 - 1 ) / 2 + ras.precision_half );
           break;
 
         default:  
           return;   
         }
 
          
          
           
          if ( pxl < 0 )
            pxl = e1;
        else if ( TRUNC( pxl ) >= ras.target.rows )
//         else if ( (ULong)( TRUNC( pxl ) ) >= ras.target.rows )
            pxl = e2;
  
           
         e1 = pxl == e1 ? e2 : e1;
 
         e1 = TRUNC( e1 );
 
         bits = ras.bTarget + ( y >> 3 );
         f1   = (Byte)( 0x80 >> ( y & 7 ) );
 
         bits -= e1 * ras.target.pitch;
          if ( ras.target.pitch > 0 )
            bits += ( ras.target.rows - 1 ) * ras.target.pitch;
  
        if ( e1 >= 0              &&
             e1 < ras.target.rows &&
             *bits & f1           )
//         if ( e1 >= 0                     &&
//              (ULong)e1 < ras.target.rows &&
//              *bits & f1                  )
            return;
        }
        else
         return;
     }
 
     bits = ras.bTarget + ( y >> 3 );
     f1   = (Byte)( 0x80 >> ( y & 7 ) );
  
      e1 = TRUNC( pxl );
  
    if ( e1 >= 0 && e1 < ras.target.rows )
//     if ( e1 >= 0 && (ULong)e1 < ras.target.rows )
      {
        bits -= e1 * ras.target.pitch;
        if ( ras.target.pitch > 0 )
         bits += ( ras.target.rows - 1 ) * ras.target.pitch;
 
       bits[0] |= f1;
     }
   }