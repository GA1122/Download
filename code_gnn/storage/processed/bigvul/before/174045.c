u32 NextPacket(u8 **pStrm)
{

    u32 index;
    u32 maxIndex;
    u32 zeroCount;
    u8 *stream;
    u8 byte;
 static u32 prevIndex=0;

  
 if (!packetize && !nalUnitStream)
 return 0;

    index = 0;
    stream = *pStrm + prevIndex;
    maxIndex = (u32)(streamStop - stream);

 if (maxIndex == 0)
 return(0);

  
 do
 {
        byte = stream[index++];
 } while (byte != 1 && index < maxIndex);

  
 if (index == maxIndex || index < 3)
 {
        DEBUG(("INVALID BYTE STREAM\n"));
        exit(100);
 }

  
 if (nalUnitStream)
 {
        stream += index;
        maxIndex -= index;
        index = 0;
 }

    zeroCount = 0;

  
  
 while (1)
 {
        byte = stream[index++];
 if (!byte)
            zeroCount++;

 if ( (byte == 0x01) && (zeroCount >= 2) )
 {
  
 if (zeroCount > 3)
 {
                index -= 4;
                zeroCount -= 3;
 }
 else
 {
                index -= zeroCount+1;
                zeroCount = 0;
 }
 break;
 }
 else if (byte)
            zeroCount = 0;

 if (index == maxIndex)
 {
 break;
 }

 }

  
 *pStrm = stream;
    prevIndex = index;

  
 if (nalUnitStream)
        index -= zeroCount;

 return(index);

}
