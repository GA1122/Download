Magick_Orientation_to_Exif_Orientation(const OrientationType orientation)
{
  switch (orientation)
  {
     

    case TopLeftOrientation:
       return 1;
    case TopRightOrientation:
       return 2;
    case BottomRightOrientation:
       return 3;
    case BottomLeftOrientation:
       return 4;
    case LeftTopOrientation:
       return 5;
    case RightTopOrientation:
       return 6;
    case RightBottomOrientation:
       return 7;
    case LeftBottomOrientation:
       return 8;
    case UndefinedOrientation:
    default:
       return 0;
  }
}
