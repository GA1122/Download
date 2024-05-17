static int guess_month(const char * message, size_t length, size_t indx)
{
  int state;

  state = MONTH_START;

  while (1) {

    if (indx >= length)
      return -1;

    switch(state) {
    case MONTH_START:
      switch((char) toupper((unsigned char) message[indx])) {
      case 'J':  
	state = MONTH_J;
	break;
      case 'F':  
	return 2;
      case 'M':  
	state = MONTH_M;
	break;
      case 'A':  
	state = MONTH_A;
	break;
      case 'S':  
	return 9;
      case 'O':  
	return 10;
      case 'N':  
	return 11;
      case 'D':  
	return 12;
      default:
	return -1;
      }
      break;
    case MONTH_J:
      switch((char) toupper((unsigned char) message[indx])) {
      case 'A':
	return 1;
      case 'U':
	state = MONTH_JU;
	break;
      default:
	return -1;
      }
      break;
    case MONTH_JU:
      switch((char) toupper((unsigned char) message[indx])) {
      case 'N':
	return 6;
      case 'L':
	return 7;
      default:
	return -1;
      }
      break;
    case MONTH_M:
      switch((char) toupper((unsigned char) message[indx])) {
      case 'A':
	state = MONTH_MA;
	break;
      default:
	return -1;
      }
      break;
    case MONTH_MA:
      switch((char) toupper((unsigned char) message[indx])) {
      case 'Y':
	return 5;
      case 'R':
	return 3;
      default:
	return -1;
      }
      break;
    case MONTH_A:
      switch((char) toupper((unsigned char) message[indx])) {
      case 'P':
	return 4;
      case 'U':
	return 8;
      default:
	return -1;
      }
      break;
    }

    indx ++;
  }
}
