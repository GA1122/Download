 TabGroupData::TabGroupData() {
   static int next_placeholder_title_number = 1;
   title_ = base::ASCIIToUTF16(
       "Group " + base::NumberToString(next_placeholder_title_number));
    ++next_placeholder_title_number;
  
    static SkRandom rand;
  stroke_color_ = rand.nextU() | 0xff000000;
//   color_ = rand.nextU() | 0xff000000;
  }