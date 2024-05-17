static void _patch_reloc (ut16 e_machine, RIOBind *iob, RBinElfReloc *rel, ut64 S, ut64 B, ut64 L) {
	ut64 val;
	ut64 A = rel->addend, P = rel->rva;
	ut8 buf[8];
	switch (e_machine) {
	case EM_PPC64: {
		int low = 0, word = 0;
		switch (rel->type) {
		case R_PPC64_REL16_HA:
			word = 2;
			val = (S + A - P + 0x8000) >> 16;
			break;
		case R_PPC64_REL16_LO:
			word = 2;
			val = (S + A - P) & 0xffff;
			break;
		case R_PPC64_REL14:
			low = 14;
			val = (st64)(S + A - P) >> 2;
			break;
		case R_PPC64_REL24:
			low = 24;
			val = (st64)(S + A - P) >> 2;
			break;
		case R_PPC64_REL32:
			word = 4;
			val = S + A - P;
			break;
		default:
			break;
		}
		if (low) {
			switch (low) {
			case 14:
				val &= (1 << 14) - 1;
				iob->read_at (iob->io, rel->rva, buf, 2);
				r_write_le32 (buf, (r_read_le32 (buf) & ~((1<<16) - (1<<2))) | val << 2);
				iob->write_at (iob->io, rel->rva, buf, 2);
				break;
			case 24:
				val &= (1 << 24) - 1;
				iob->read_at (iob->io, rel->rva, buf, 4);
				r_write_le32 (buf, (r_read_le32 (buf) & ~((1<<26) - (1<<2))) | val << 2);
				iob->write_at (iob->io, rel->rva, buf, 4);
				break;
			}
		} else if (word) {
			switch (word) {
			case 2:
				r_write_le16 (buf, val);
				iob->write_at (iob->io, rel->rva, buf, 2);
				break;
			case 4:
				r_write_le32 (buf, val);
				iob->write_at (iob->io, rel->rva, buf, 4);
				break;
			}
		}
		break;
	}
	case EM_X86_64: {
		int word = 0;
		switch (rel->type) {
		case R_X86_64_8:
			word = 1;
			val = S + A;
			break;
		case R_X86_64_16:
			word = 2;
			val = S + A;
			break;
		case R_X86_64_32:
		case R_X86_64_32S:
			word = 4;
			val = S + A;
			break;
		case R_X86_64_64:
			word = 8;
			val = S + A;
			break;
		case R_X86_64_GLOB_DAT:
		case R_X86_64_JUMP_SLOT:
			word = 4;
			val = S;
			break;
		case R_X86_64_PC8:
			word = 1;
			val = S + A - P;
			break;
		case R_X86_64_PC16:
			word = 2;
			val = S + A - P;
			break;
		case R_X86_64_PC32:
			word = 4;
			val = S + A - P;
			break;
		case R_X86_64_PC64:
			word = 8;
			val = S + A - P;
			break;
		case R_X86_64_PLT32:
			word = 4;
			val = L + A - P;
			break;
		case R_X86_64_RELATIVE:
			word = 8;
			val = B + A;
			break;
		default:
			break;
		}
		switch (word) {
		case 0:
			break;
		case 1:
			buf[0] = val;
			iob->write_at (iob->io, rel->rva, buf, 1);
			break;
		case 2:
			r_write_le16 (buf, val);
			iob->write_at (iob->io, rel->rva, buf, 2);
			break;
		case 4:
			r_write_le32 (buf, val);
			iob->write_at (iob->io, rel->rva, buf, 4);
			break;
		case 8:
			r_write_le64 (buf, val);
			iob->write_at (iob->io, rel->rva, buf, 8);
			break;
		}
		break;
	}
	}
}