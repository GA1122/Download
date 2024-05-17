static void write_register_operand(struct operand *op)
{
	 
	switch (op->bytes) {
	case 1:
		*(u8 *)op->addr.reg = (u8)op->val;
		break;
	case 2:
		*(u16 *)op->addr.reg = (u16)op->val;
		break;
	case 4:
		*op->addr.reg = (u32)op->val;
		break;	 
	case 8:
		*op->addr.reg = op->val;
		break;
	}
}