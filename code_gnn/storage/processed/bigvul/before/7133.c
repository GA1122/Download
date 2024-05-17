  cf2_getWindingMomentum( CF2_Fixed  x1,
                          CF2_Fixed  y1,
                          CF2_Fixed  x2,
                          CF2_Fixed  y2 )
  {
     
     

    return ( x1 >> 16 ) * ( ( y2 - y1 ) >> 16 ) -
           ( y1 >> 16 ) * ( ( x2 - x1 ) >> 16 );
  }
