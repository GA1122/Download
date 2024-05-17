  TT_MulFix14_arm( FT_Int32  a,
                   FT_Int    b )
  {
    FT_Int32  t, t2;


#if defined( __CC_ARM ) || defined( __ARMCC__ )

    __asm
    {
      smull t2, t,  b,  a            
      mov   a,  t,  asr #31          
      add   a,  a,  #0x2000          
      adds  t2, t2, a                
      adc   t,  t,  #0               
      mov   a,  t2, lsr #14          
      orr   a,  a,  t,  lsl #18      
    }

#elif defined( __GNUC__ )

    __asm__ __volatile__ (
      "smull  %1, %2, %4, %3\n\t"        
      "mov    %0, %2, asr #31\n\t"       
#if defined( __clang__ ) && defined( __thumb2__ )
      "add.w  %0, %0, #0x2000\n\t"       
#else
      "add    %0, %0, #0x2000\n\t"       
#endif
      "adds   %1, %1, %0\n\t"            
      "adc    %2, %2, #0\n\t"            
      "mov    %0, %1, lsr #14\n\t"       
      "orr    %0, %0, %2, lsl #18\n\t"   
      : "=r"(a), "=&r"(t2), "=&r"(t)
      : "r"(a), "r"(b)
      : "cc" );

#endif

    return a;
  }
