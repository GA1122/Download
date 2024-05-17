static UINT8 opcode_from_pdu(UINT8 pdu)
{
    UINT8 opcode = 0;

 switch (pdu)
 {
 case AVRC_PDU_NEXT_GROUP:
 case AVRC_PDU_PREV_GROUP:  
        opcode  = AVRC_OP_PASS_THRU;
 break;

 default:  
        opcode  = AVRC_OP_VENDOR;
 break;
 }

 return opcode;
}
