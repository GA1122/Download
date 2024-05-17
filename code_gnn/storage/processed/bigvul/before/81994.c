static void _6502_anal_esil_ccall(RAnalOp *op, ut8 data0)
{
	char *flag;
	switch(data0) {
	case 0x10:  
		flag = "N,!";
		break;
	case 0x30:  
		flag = "N";
		break;
	case 0x50:  
		flag = "V,!";
		break;
	case 0x70:  
		flag = "V";
		break;
	case 0x90:  
		flag = "C,!";
		break;
	case 0xb0:  
		flag = "C";
		break;
	case 0xd0:  
		flag = "Z,!";
		break;
	case 0xf0:  
		flag = "Z";
		break;
	default:
		flag = "unk";
		break;
	}
	r_strbuf_setf (&op->esil, "%s,?{,0x%04x,pc,=,}", flag, (op->jump & 0xffff));
}
