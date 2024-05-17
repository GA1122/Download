Pin NO_INLINE jshFindPinForFunction(JshPinFunction functionType, JshPinFunction functionInfo) {
#ifdef OLIMEXINO_STM32
   
  if (functionType == JSH_SPI1 && functionInfo==JSH_SPI_MOSI) return JSH_PORTD_OFFSET+11;
#endif
#ifdef PICO
   
  if (functionType == JSH_USART1) {
    if (functionInfo==JSH_USART_TX) return JSH_PORTB_OFFSET+6;
    if (functionInfo==JSH_USART_RX) return JSH_PORTB_OFFSET+7;
  }
#endif
  Pin i;
  int j;
  for (i=0;i<JSH_PIN_COUNT;i++)
    for (j=0;j<JSH_PININFO_FUNCTIONS;j++)
      if ((pinInfo[i].functions[j]&JSH_MASK_AF) == JSH_AF0 &&
          (pinInfo[i].functions[j]&JSH_MASK_TYPE) == functionType &&
          (pinInfo[i].functions[j]&JSH_MASK_INFO) == functionInfo)
        return i;
  for (i=0;i<JSH_PIN_COUNT;i++)
    for (j=0;j<JSH_PININFO_FUNCTIONS;j++)
      if ((pinInfo[i].functions[j]&JSH_MASK_TYPE) == functionType &&
          (pinInfo[i].functions[j]&JSH_MASK_INFO) == functionInfo)
        return i;
  return PIN_UNDEFINED;
}
