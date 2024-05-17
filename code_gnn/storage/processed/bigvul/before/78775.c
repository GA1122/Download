static unsigned int map_operations (int commandbyte )
{
	unsigned int op = (unsigned int)-1;

	switch ( (commandbyte & 0xfe) ) {
		case 0xe2:     op = SC_AC_OP_UPDATE; break;
		case 0x24:   op = SC_AC_OP_UPDATE; break;
		case 0xe0:            op = SC_AC_OP_CREATE; break;
		case 0xe4:            op = SC_AC_OP_DELETE; break;
		case 0xe8:       op = SC_AC_OP_WRITE; break;
		case 0x82:     op = SC_AC_OP_READ; break;
		case 0xe6:       op = SC_AC_OP_WRITE; break;
		case 0x88:     op = SC_AC_OP_READ; break;
		case 0x04:        op = SC_AC_OP_INVALIDATE; break;
		case 0x2a:   op = SC_AC_OP_SELECT; break;
		case 0xb0:       op = SC_AC_OP_READ; break;
		case 0xb2:       op = SC_AC_OP_READ; break;
		case 0x44:      op = SC_AC_OP_REHABILITATE; break;
		case 0xa4:            op = SC_AC_OP_SELECT; break;
		case 0xee:     op = SC_AC_OP_CREATE; break;
		case 0x2c:  op = SC_AC_OP_WRITE; break;
		case 0xd6:     op = SC_AC_OP_WRITE; break;
		case 0xdc:     op = SC_AC_OP_WRITE; break;
		case 0x20:   op = SC_AC_OP_SELECT; break;
		case 0x60:       op = SC_AC_OP_CREATE; break;
	}
	return op;
}
