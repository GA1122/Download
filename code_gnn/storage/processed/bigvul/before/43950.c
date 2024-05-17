int xen_pcibk_config_add_field_offset(struct pci_dev *dev,
				    const struct config_field *field,
				    unsigned int base_offset)
{
	int err = 0;
	struct xen_pcibk_dev_data *dev_data = pci_get_drvdata(dev);
	struct config_field_entry *cfg_entry;
	void *tmp;

	cfg_entry = kmalloc(sizeof(*cfg_entry), GFP_KERNEL);
	if (!cfg_entry) {
		err = -ENOMEM;
		goto out;
	}

	cfg_entry->data = NULL;
	cfg_entry->field = field;
	cfg_entry->base_offset = base_offset;

	 
	err = xen_pcibk_field_is_dup(dev, OFFSET(cfg_entry));
	if (err)
		goto out;

	if (field->init) {
		tmp = field->init(dev, OFFSET(cfg_entry));

		if (IS_ERR(tmp)) {
			err = PTR_ERR(tmp);
			goto out;
		}

		cfg_entry->data = tmp;
	}

	dev_dbg(&dev->dev, "added config field at offset 0x%02x\n",
		OFFSET(cfg_entry));
	list_add_tail(&cfg_entry->list, &dev_data->config_fields);

out:
	if (err)
		kfree(cfg_entry);

	return err;
}