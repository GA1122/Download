static uint64_t srpt_unpack_lun(const uint8_t *lun, int len)
{
	uint64_t res = NO_SUCH_LUN;
	int addressing_method;

	if (unlikely(len < 2)) {
		pr_err("Illegal LUN length %d, expected 2 bytes or more\n",
		       len);
		goto out;
	}

	switch (len) {
	case 8:
		if ((*((__be64 *)lun) &
		     cpu_to_be64(0x0000FFFFFFFFFFFFLL)) != 0)
			goto out_err;
		break;
	case 4:
		if (*((__be16 *)&lun[2]) != 0)
			goto out_err;
		break;
	case 6:
		if (*((__be32 *)&lun[2]) != 0)
			goto out_err;
		break;
	case 2:
		break;
	default:
		goto out_err;
	}

	addressing_method = (*lun) >> 6;  
	switch (addressing_method) {
	case SCSI_LUN_ADDR_METHOD_PERIPHERAL:
	case SCSI_LUN_ADDR_METHOD_FLAT:
	case SCSI_LUN_ADDR_METHOD_LUN:
		res = *(lun + 1) | (((*lun) & 0x3f) << 8);
		break;

	case SCSI_LUN_ADDR_METHOD_EXTENDED_LUN:
	default:
		pr_err("Unimplemented LUN addressing method %u\n",
		       addressing_method);
		break;
	}

out:
	return res;

out_err:
	pr_err("Support for multi-level LUNs has not yet been implemented\n");
	goto out;
}