static MediaContainerName LookupContainerByFirst4(const uint8* buffer,
                                                  int buffer_size) {
  if (buffer_size < 12)
    return CONTAINER_UNKNOWN;

  uint32 first4 = Read32(buffer);
  switch (first4) {
    case 0x1a45dfa3:
      if (CheckWebm(buffer, buffer_size))
        return CONTAINER_WEBM;
      break;

    case 0x3026b275:
      if (StartsWith(buffer,
                     buffer_size,
                     kAsfSignature,
                     sizeof(kAsfSignature))) {
        return CONTAINER_ASF;
      }
      break;

    case TAG('#','!','A','M'):
      if (StartsWith(buffer, buffer_size, kAmrSignature))
        return CONTAINER_AMR;
      break;

    case TAG('#','E','X','T'):
      if (CheckHls(buffer, buffer_size))
        return CONTAINER_HLS;
      break;

    case TAG('.','R','M','F'):
      if (buffer[4] == 0 && buffer[5] == 0)
        return CONTAINER_RM;
      break;

    case TAG('.','r','a','\xfd'):
      return CONTAINER_RM;

    case TAG('B','I','K','b'):
    case TAG('B','I','K','d'):
    case TAG('B','I','K','f'):
    case TAG('B','I','K','g'):
    case TAG('B','I','K','h'):
    case TAG('B','I','K','i'):
      if (CheckBink(buffer, buffer_size))
        return CONTAINER_BINK;
      break;

    case TAG('c','a','f','f'):
      if (CheckCaf(buffer, buffer_size))
        return CONTAINER_CAF;
      break;

    case TAG('D','E','X','A'):
      if (buffer_size > 15 &&
          Read16(buffer + 11) <= 2048 &&
          Read16(buffer + 13) <= 2048) {
        return CONTAINER_DXA;
      }
      break;

    case TAG('D','T','S','H'):
      if (Read32(buffer + 4) == TAG('D','H','D','R'))
        return CONTAINER_DTSHD;
      break;

    case 0x64a30100:
    case 0x64a30200:
    case 0x64a30300:
    case 0x64a30400:
    case 0x0001a364:
    case 0x0002a364:
    case 0x0003a364:
      if (Read32(buffer + 4) != 0 && Read32(buffer + 8) != 0)
        return CONTAINER_IRCAM;
      break;

    case TAG('f','L','a','C'):
      return CONTAINER_FLAC;

    case TAG('F','L','V',0):
    case TAG('F','L','V',1):
    case TAG('F','L','V',2):
    case TAG('F','L','V',3):
    case TAG('F','L','V',4):
      if (buffer[5] == 0 && Read32(buffer + 5) > 8)
        return CONTAINER_FLV;
      break;

    case TAG('F','O','R','M'):
      switch (Read32(buffer + 8)) {
        case TAG('A','I','F','F'):
        case TAG('A','I','F','C'):
          return CONTAINER_AIFF;
      }
      break;

    case TAG('M','A','C',' '):
      return CONTAINER_APE;

    case TAG('O','N','2',' '):
      if (Read32(buffer + 8) == TAG('O','N','2','f'))
        return CONTAINER_AVI;
      break;

    case TAG('O','g','g','S'):
      if (buffer[5] <= 7)
        return CONTAINER_OGG;
      break;

    case TAG('R','F','6','4'):
      if (buffer_size > 16 && Read32(buffer + 12) == TAG('d','s','6','4'))
        return CONTAINER_WAV;
      break;

    case TAG('R','I','F','F'):
      switch (Read32(buffer + 8)) {
        case TAG('A','V','I',' '):
        case TAG('A','V','I','X'):
        case TAG('A','V','I','\x19'):
        case TAG('A','M','V',' '):
          return CONTAINER_AVI;
        case TAG('W','A','V','E'):
          return CONTAINER_WAV;
      }
      break;

    case TAG('[','S','c','r'):
      if (StartsWith(buffer, buffer_size, kAssSignature))
        return CONTAINER_ASS;
      break;

    case TAG('\xef','\xbb','\xbf','['):
      if (StartsWith(buffer, buffer_size, kAssBomSignature))
        return CONTAINER_ASS;
      break;

    case 0x7ffe8001:
    case 0xfe7f0180:
    case 0x1fffe800:
    case 0xff1f00e8:
      if (CheckDts(buffer, buffer_size))
        return CONTAINER_DTS;
      break;

    case 0xb7d80020:
      if (StartsWith(buffer,
                     buffer_size,
                     kWtvSignature,
                     sizeof(kWtvSignature))) {
        return CONTAINER_WTV;
      }
      break;
  }

  uint32 first3 = first4 & 0xffffff00;
  switch (first3) {
    case TAG('C','W','S',0):
    case TAG('F','W','S',0):
      return CONTAINER_SWF;

    case TAG('I','D','3',0):
      if (CheckMp3(buffer, buffer_size, true))
        return CONTAINER_MP3;
      break;
  }

  uint32 first2 = Read16(buffer);
  switch (first2) {
    case kAc3SyncWord:
      if (CheckAc3(buffer, buffer_size))
        return CONTAINER_AC3;
      if (CheckEac3(buffer, buffer_size))
        return CONTAINER_EAC3;
      break;

    case 0xfff0:
    case 0xfff1:
    case 0xfff8:
    case 0xfff9:
      if (CheckAac(buffer, buffer_size))
        return CONTAINER_AAC;
      break;
  }

  if (CheckMp3(buffer, buffer_size, false))
    return CONTAINER_MP3;

  return CONTAINER_UNKNOWN;
}
