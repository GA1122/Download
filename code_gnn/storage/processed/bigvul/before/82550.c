Pin jshGetPinFromString(const char *s) {
  if (((s[0]>='A' && s[0]<='I') || s[0]=='V') && s[1]) {
    int port = (s[0]=='V') ? JSH_PORTV : JSH_PORTA+s[0]-'A';
    int pin = -1;
    if (s[1]>='0' && s[1]<='9') {
      if (!s[2]) {  
        pin = (s[1]-'0');
      } else if (s[2]>='0' && s[2]<='9') {
        if (!s[3]) {
          pin = ((s[1]-'0')*10 + (s[2]-'0'));
#ifdef LINUX
        } else if (!s[4] && s[3]>='0' && s[3]<='9') {
          pin = ((s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0'));
#endif
        }
      }
    }
    if (pin>=0) {
#ifdef PIN_NAMES_DIRECT
      int i;
      for (i=0;i<JSH_PIN_COUNT;i++)
        if ((pinInfo[i].port&JSH_PORT_MASK) == port && pinInfo[i].pin==pin)
          return (Pin)i;
#else
      if (0) {
#if JSH_PORTA_OFFSET!=-1
      } else if (port == JSH_PORTA) {
        if (pin<JSH_PORTA_COUNT) return (Pin)(JSH_PORTA_OFFSET + pin);
#endif
#if JSH_PORTB_OFFSET!=-1
      } else if (port == JSH_PORTB) {
        if (pin<JSH_PORTB_COUNT) return (Pin)(JSH_PORTB_OFFSET + pin);
#endif
#if JSH_PORTC_OFFSET!=-1
      } else if (port == JSH_PORTC) {
        if (pin<JSH_PORTC_COUNT) return (Pin)(JSH_PORTC_OFFSET + pin);
#endif
#if JSH_PORTD_OFFSET!=-1
      } else if (port == JSH_PORTD) {
        if (pin<JSH_PORTD_COUNT) return (Pin)(JSH_PORTD_OFFSET + pin);
#endif
#if JSH_PORTE_OFFSET!=-1
      } else if (port == JSH_PORTE) {
        if (pin<JSH_PORTE_COUNT) return (Pin)(JSH_PORTE_OFFSET + pin);
#endif
#if JSH_PORTF_OFFSET!=-1
      } else if (port == JSH_PORTF) {
        if (pin<JSH_PORTF_COUNT) return (Pin)(JSH_PORTF_OFFSET + pin);
#endif
#if JSH_PORTG_OFFSET!=-1
      } else if (port == JSH_PORTG) {
        if (pin<JSH_PORTG_COUNT) return (Pin)(JSH_PORTG_OFFSET + pin);
#endif
#if JSH_PORTH_OFFSET!=-1
      } else if (port == JSH_PORTH) {
        if (pin<JSH_PORTH_COUNT) return (Pin)(JSH_PORTH_OFFSET + pin);
#endif
#if JSH_PORTI_OFFSET!=-1
      } else if (port == JSH_PORTI) {
        if (pin<JSH_PORTI_COUNT) return (Pin)(JSH_PORTI_OFFSET + pin);
#endif
#if JSH_PORTV_OFFSET!=-1
      } else if (port == JSH_PORTV) {
        if (pin<JSH_PORTV_COUNT) return (Pin)(JSH_PORTV_OFFSET + pin);
#endif
      }
#endif
    }
  }

  return PIN_UNDEFINED;
}