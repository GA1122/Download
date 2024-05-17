int xen_pcibk_config_read(struct pci_dev *dev, int offset, int size,
			  u32 *ret_val)
{
	int err = 0;
	struct xen_pcibk_dev_data *dev_data = pci_get_drvdata(dev);
	const struct config_field_entry *cfg_entry;
	const struct config_field *field;
	int req_start, req_end, field_start, field_end;
	 
	u32 value = 0, tmp_val;

	if (unlikely(verbose_request))
		printk(KERN_DEBUG DRV_NAME ": %s: read %d bytes at 0x%x\n",
		       pci_name(dev), size, offset);

	if (!valid_request(offset, size)) {
		err = XEN_PCI_ERR_invalid_offset;
		goto out;
	}

	 
	switch (size) {
	case 1:
		err = pci_read_config_byte(dev, offset, (u8 *) &value);
		break;
	case 2:
		err = pci_read_config_word(dev, offset, (u16 *) &value);
		break;
	case 4:
		err = pci_read_config_dword(dev, offset, &value);
		break;
	}

	list_for_each_entry(cfg_entry, &dev_data->config_fields, list) {
		field = cfg_entry->field;

		req_start = offset;
		req_end = offset + size;
		field_start = OFFSET(cfg_entry);
		field_end = OFFSET(cfg_entry) + field->size;

		if ((req_start >= field_start && req_start < field_end)
		    || (req_end > field_start && req_end <= field_end)) {
			err = conf_space_read(dev, cfg_entry, field_start,
					      &tmp_val);
			if (err)
				goto out;

			value = merge_value(value, tmp_val,
					    get_mask(field->size),
					    field_start - req_start);
		}
	}

out:
	if (unlikely(verbose_request))
		printk(KERN_DEBUG DRV_NAME ": %s: read %d bytes at 0x%x = %x\n",
		       pci_name(dev), size, offset, value);

	*ret_val = value;
	return xen_pcibios_err_to_errno(err);
}