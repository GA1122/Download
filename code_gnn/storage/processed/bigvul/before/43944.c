static int conf_space_read(struct pci_dev *dev,
			   const struct config_field_entry *entry,
			   int offset, u32 *value)
{
	int ret = 0;
	const struct config_field *field = entry->field;

	*value = 0;

	switch (field->size) {
	case 1:
		if (field->u.b.read)
			ret = field->u.b.read(dev, offset, (u8 *) value,
					      entry->data);
		break;
	case 2:
		if (field->u.w.read)
			ret = field->u.w.read(dev, offset, (u16 *) value,
					      entry->data);
		break;
	case 4:
		if (field->u.dw.read)
			ret = field->u.dw.read(dev, offset, value, entry->data);
		break;
	}
	return ret;
}