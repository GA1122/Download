static int do_download_mode(struct edgeport_serial *serial,
		const struct firmware *fw)
{
	struct device *dev = &serial->serial->interface->dev;
	int status = 0;
	int start_address;
	struct edge_ti_manuf_descriptor *ti_manuf_desc;
	int download_cur_ver;
	int download_new_ver;
	struct edgeport_fw_hdr *fw_hdr = (struct edgeport_fw_hdr *)fw->data;
	struct ti_i2c_desc *rom_desc;

	dev_dbg(dev, "%s - RUNNING IN DOWNLOAD MODE\n", __func__);

	status = check_i2c_image(serial);
	if (status) {
		dev_dbg(dev, "%s - DOWNLOAD MODE -- BAD I2C\n", __func__);
		return status;
	}

	 
	ti_manuf_desc = kmalloc(sizeof(*ti_manuf_desc), GFP_KERNEL);
	if (!ti_manuf_desc)
		return -ENOMEM;

	status = get_manuf_info(serial, (__u8 *)ti_manuf_desc);
	if (status) {
		kfree(ti_manuf_desc);
		return status;
	}

	 
	if (!ignore_cpu_rev && ti_cpu_rev(ti_manuf_desc) < 2) {
		dev_dbg(dev, "%s - Wrong CPU Rev %d (Must be 2)\n",
			__func__, ti_cpu_rev(ti_manuf_desc));
		kfree(ti_manuf_desc);
		return -EINVAL;
	}

	rom_desc = kmalloc(sizeof(*rom_desc), GFP_KERNEL);
	if (!rom_desc) {
		kfree(ti_manuf_desc);
		return -ENOMEM;
	}

	 
	start_address = get_descriptor_addr(serial,
			I2C_DESC_TYPE_FIRMWARE_BASIC, rom_desc);
	if (start_address != 0) {
		struct ti_i2c_firmware_rec *firmware_version;
		u8 *record;

		dev_dbg(dev, "%s - Found Type FIRMWARE (Type 2) record\n",
				__func__);

		firmware_version = kmalloc(sizeof(*firmware_version),
							GFP_KERNEL);
		if (!firmware_version) {
			kfree(rom_desc);
			kfree(ti_manuf_desc);
			return -ENOMEM;
		}

		 
		status = read_rom(serial, start_address +
				sizeof(struct ti_i2c_desc),
				sizeof(struct ti_i2c_firmware_rec),
				(__u8 *)firmware_version);
		if (status) {
			kfree(firmware_version);
			kfree(rom_desc);
			kfree(ti_manuf_desc);
			return status;
		}

		 
		download_cur_ver = (firmware_version->Ver_Major << 8) +
				   (firmware_version->Ver_Minor);
		download_new_ver = (fw_hdr->major_version << 8) +
				   (fw_hdr->minor_version);

		dev_dbg(dev, "%s - >> FW Versions Device %d.%d  Driver %d.%d\n",
			__func__, firmware_version->Ver_Major,
			firmware_version->Ver_Minor,
			fw_hdr->major_version, fw_hdr->minor_version);

		 
		if (download_cur_ver < download_new_ver) {
			dev_dbg(dev, "%s - Update I2C dld from %d.%d to %d.%d\n",
				__func__,
				firmware_version->Ver_Major,
				firmware_version->Ver_Minor,
				fw_hdr->major_version,
				fw_hdr->minor_version);

			record = kmalloc(1, GFP_KERNEL);
			if (!record) {
				kfree(firmware_version);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return -ENOMEM;
			}
			 
			*record = I2C_DESC_TYPE_FIRMWARE_BLANK;

			 
			status = write_rom(serial, start_address,
					sizeof(*record), record);
			if (status) {
				kfree(record);
				kfree(firmware_version);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return status;
			}

			 
			status = read_rom(serial,
						start_address,
						sizeof(*record),
						record);
			if (status) {
				kfree(record);
				kfree(firmware_version);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return status;
			}

			if (*record != I2C_DESC_TYPE_FIRMWARE_BLANK) {
				dev_err(dev, "%s - error resetting device\n",
						__func__);
				kfree(record);
				kfree(firmware_version);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return -ENODEV;
			}

			dev_dbg(dev, "%s - HARDWARE RESET\n", __func__);

			 
			status = ti_vsend_sync(serial->serial->dev,
					UMPC_HARDWARE_RESET,
					0, 0, NULL, 0,
					TI_VSEND_TIMEOUT_DEFAULT);

			dev_dbg(dev, "%s - HARDWARE RESET return %d\n",
					__func__, status);

			 
			kfree(record);
			kfree(firmware_version);
			kfree(rom_desc);
			kfree(ti_manuf_desc);
			return -ENODEV;
		}
		 
		serial->fw_version = download_cur_ver;
		kfree(firmware_version);
	}
	 
	else {
		start_address = get_descriptor_addr(serial,
				I2C_DESC_TYPE_FIRMWARE_BLANK, rom_desc);
		if (start_address != 0) {
#define HEADER_SIZE	(sizeof(struct ti_i2c_desc) + \
				sizeof(struct ti_i2c_firmware_rec))
			__u8 *header;
			__u8 *vheader;

			header = kmalloc(HEADER_SIZE, GFP_KERNEL);
			if (!header) {
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return -ENOMEM;
			}

			vheader = kmalloc(HEADER_SIZE, GFP_KERNEL);
			if (!vheader) {
				kfree(header);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return -ENOMEM;
			}

			dev_dbg(dev, "%s - Found Type BLANK FIRMWARE (Type F2) record\n",
					__func__);

			 
			status = build_i2c_fw_hdr(header, fw);
			if (status) {
				kfree(vheader);
				kfree(header);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return -EINVAL;
			}

			 
			status = write_rom(serial,
						start_address,
						HEADER_SIZE,
						header);
			if (status) {
				kfree(vheader);
				kfree(header);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return -EINVAL;
			}

			 
			status = read_rom(serial, start_address,
							HEADER_SIZE, vheader);

			if (status) {
				dev_dbg(dev, "%s - can't read header back\n",
						__func__);
				kfree(vheader);
				kfree(header);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return status;
			}
			if (memcmp(vheader, header, HEADER_SIZE)) {
				dev_dbg(dev, "%s - write download record failed\n",
						__func__);
				kfree(vheader);
				kfree(header);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return -EINVAL;
			}

			kfree(vheader);
			kfree(header);

			dev_dbg(dev, "%s - Start firmware update\n", __func__);

			 
			status = ti_vsend_sync(serial->serial->dev,
					UMPC_COPY_DNLD_TO_I2C,
					0, 0, NULL, 0,
					TI_VSEND_TIMEOUT_FW_DOWNLOAD);

			dev_dbg(dev, "%s - Update complete 0x%x\n", __func__,
					status);
			if (status) {
				dev_err(dev,
					"%s - UMPC_COPY_DNLD_TO_I2C failed\n",
					__func__);
				kfree(rom_desc);
				kfree(ti_manuf_desc);
				return status;
			}
		}
	}

	 
	kfree(rom_desc);
	kfree(ti_manuf_desc);
	return 0;
}
