static bool isCompatibleBrand(uint32_t fourcc) {
 static const uint32_t kCompatibleBrands[] = {
        FOURCC('i', 's', 'o', 'm'),
        FOURCC('i', 's', 'o', '2'),
        FOURCC('a', 'v', 'c', '1'),
        FOURCC('3', 'g', 'p', '4'),
        FOURCC('m', 'p', '4', '1'),
        FOURCC('m', 'p', '4', '2'),

        FOURCC('q', 't', ' ', ' '),  
        FOURCC('M', 'S', 'N', 'V'),  

        FOURCC('3', 'g', '2', 'a'),  
        FOURCC('3', 'g', '2', 'b'),
 };

 for (size_t i = 0;
         i < sizeof(kCompatibleBrands) / sizeof(kCompatibleBrands[0]);
 ++i) {
 if (kCompatibleBrands[i] == fourcc) {
 return true;
 }
 }

 return false;
}
