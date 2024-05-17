 test_size(png_modifier* PNG_CONST pm, png_byte PNG_CONST colour_type,
    int bdlo, int PNG_CONST bdhi)
// test_size(png_modifier* const pm, png_byte const colour_type,
//     int bdlo, int const bdhi)
  {
      
   static PNG_CONST png_byte hinc[] = {1, 3, 11, 1, 5};
   static PNG_CONST png_byte winc[] = {1, 9, 5, 7, 1};
//    static const png_byte hinc[] = {1, 3, 11, 1, 5};
//    static const png_byte winc[] = {1, 9, 5, 7, 1};
//    const int save_bdlo = bdlo;
// 
     for (; bdlo <= bdhi; ++bdlo)
     {
        png_uint_32 h, w;
 
  for (h=1; h<=16; h+=hinc[bdlo]) for (w=1; w<=16; w+=winc[bdlo])
  {
   
          standard_test(&pm->this, FILEID(colour_type, DEPTH(bdlo), 0 ,
             PNG_INTERLACE_NONE, w, h, 0), 0 ,
             pm->use_update_info);
 
  if (fail(pm))
  return 0;
 
          standard_test(&pm->this, FILEID(colour_type, DEPTH(bdlo), 0 ,
             PNG_INTERLACE_NONE, w, h, 1), 0 ,
             pm->use_update_info);
 
 
           if (fail(pm))
              return 0;
  
#     ifdef PNG_WRITE_INTERLACING_SUPPORTED
//            
          standard_test(&pm->this, FILEID(colour_type, DEPTH(bdlo), 0 ,
            PNG_INTERLACE_ADAM7, w, h, 0), 0 ,
//             PNG_INTERLACE_NONE, w, h, 0), 1 ,
             pm->use_update_info);
         if (fail(pm))
            return 0;
// 
// 
//           if (fail(pm))
//              return 0;
//  
// #     if CAN_WRITE_INTERLACE
//           
          standard_test(&pm->this, FILEID(colour_type, DEPTH(bdlo), 0 ,
            PNG_INTERLACE_ADAM7, w, h, 1), 0 ,
//             PNG_INTERLACE_ADAM7, w, h, 1), 1 ,
             pm->use_update_info);
// 
          if (fail(pm))
             return 0;
 #     endif
           
//       }
//    }
// 
//     
//    for (bdlo = save_bdlo; bdlo <= bdhi; ++bdlo)
//    {
//       png_uint_32 h, w;
// 
//       for (h=1; h<=16; h+=hinc[bdlo]) for (w=1; w<=16; w+=winc[bdlo])
//       {
// #     ifdef PNG_READ_INTERLACING_SUPPORTED
//           
// #     if CAN_WRITE_INTERLACE
          standard_test(&pm->this, FILEID(colour_type, DEPTH(bdlo), 0 ,
            PNG_INTERLACE_NONE, w, h, 0), 1 ,
//             PNG_INTERLACE_ADAM7, w, h, 1), 0 ,
             pm->use_update_info);
 
//          if (fail(pm))
//             return 0;
// #     endif
// #     endif  
 
          if (fail(pm))
             return 0;
 
  #     ifdef PNG_WRITE_INTERLACING_SUPPORTED
//           
           standard_test(&pm->this, FILEID(colour_type, DEPTH(bdlo), 0 ,
              PNG_INTERLACE_ADAM7, w, h, 0), 1 ,
              pm->use_update_info);
 
 
           if (fail(pm))
              return 0;
  #     endif
// 
// #     ifdef PNG_READ_INTERLACING_SUPPORTED
// #     ifdef PNG_WRITE_INTERLACING_SUPPORTED
//           
//          standard_test(&pm->this, FILEID(colour_type, DEPTH(bdlo), 0 ,
//             PNG_INTERLACE_ADAM7, w, h, 0), 0 ,
//             pm->use_update_info);
// 
//          if (fail(pm))
//             return 0;
// #     endif
// #     endif  
        }
     }
  
  return 1;  
 }