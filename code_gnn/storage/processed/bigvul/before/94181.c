static bool radeon_atom_apply_quirks(struct drm_device *dev,
				     uint32_t supported_device,
				     int *connector_type,
				     struct radeon_i2c_bus_rec *i2c_bus,
				     uint16_t *line_mux,
				     struct radeon_hpd *hpd)
{

	 
	if ((dev->pdev->device == 0x791e) &&
	    (dev->pdev->subsystem_vendor == 0x1043) &&
	    (dev->pdev->subsystem_device == 0x826d)) {
		if ((*connector_type == DRM_MODE_CONNECTOR_HDMIA) &&
		    (supported_device == ATOM_DEVICE_DFP3_SUPPORT))
			*connector_type = DRM_MODE_CONNECTOR_DVID;
	}

	 
	if ((dev->pdev->device == 0x7941) &&
	    (dev->pdev->subsystem_vendor == 0x1849) &&
	    (dev->pdev->subsystem_device == 0x7941)) {
		if ((*connector_type == DRM_MODE_CONNECTOR_HDMIA) &&
		    (supported_device == ATOM_DEVICE_DFP3_SUPPORT))
			*connector_type = DRM_MODE_CONNECTOR_DVID;
	}

	 
	if ((dev->pdev->device == 0x7941) &&
	    (dev->pdev->subsystem_vendor == 0x147b) &&
	    (dev->pdev->subsystem_device == 0x2412)) {
		if (*connector_type == DRM_MODE_CONNECTOR_DVII)
			return false;
	}

	 
	if ((dev->pdev->device == 0x5653) &&
	    (dev->pdev->subsystem_vendor == 0x1462) &&
	    (dev->pdev->subsystem_device == 0x0291)) {
		if (*connector_type == DRM_MODE_CONNECTOR_LVDS) {
			i2c_bus->valid = false;
			*line_mux = 53;
		}
	}

	 
	if ((dev->pdev->device == 0x7146) &&
	    (dev->pdev->subsystem_vendor == 0x17af) &&
	    (dev->pdev->subsystem_device == 0x2058)) {
		if (supported_device == ATOM_DEVICE_DFP1_SUPPORT)
			return false;
	}

	 
	if ((dev->pdev->device == 0x7142) &&
	    (dev->pdev->subsystem_vendor == 0x1458) &&
	    (dev->pdev->subsystem_device == 0x2134)) {
		if (supported_device == ATOM_DEVICE_DFP1_SUPPORT)
			return false;
	}


	 
	if ((dev->pdev->device == 0x71C5) &&
	    (dev->pdev->subsystem_vendor == 0x106b) &&
	    (dev->pdev->subsystem_device == 0x0080)) {
		if ((supported_device == ATOM_DEVICE_CRT1_SUPPORT) ||
		    (supported_device == ATOM_DEVICE_DFP2_SUPPORT))
			return false;
		if (supported_device == ATOM_DEVICE_CRT2_SUPPORT)
			*line_mux = 0x90;
	}

	 
	if ((dev->pdev->device == 0x9598) &&
	    (dev->pdev->subsystem_vendor == 0x1043) &&
	    (dev->pdev->subsystem_device == 0x01da)) {
		if (*connector_type == DRM_MODE_CONNECTOR_HDMIA) {
			*connector_type = DRM_MODE_CONNECTOR_DVII;
		}
	}

	 
	if ((dev->pdev->device == 0x95C5) &&
	    (dev->pdev->subsystem_vendor == 0x1043) &&
	    (dev->pdev->subsystem_device == 0x01e2)) {
		if (*connector_type == DRM_MODE_CONNECTOR_HDMIA) {
			*connector_type = DRM_MODE_CONNECTOR_DVII;
		}
	}

	 
	if (*connector_type == DRM_MODE_CONNECTOR_HDMIA) {
		if (supported_device & (ATOM_DEVICE_CRT_SUPPORT)) {
			*connector_type = DRM_MODE_CONNECTOR_VGA;
			*line_mux = 0;
		}
	}

	 
	if ((dev->pdev->device == 0x95c4) &&
	    (dev->pdev->subsystem_vendor == 0x1025) &&
	    (dev->pdev->subsystem_device == 0x013c)) {
		if ((*connector_type == DRM_MODE_CONNECTOR_DVII) &&
		    (supported_device == ATOM_DEVICE_DFP1_SUPPORT))
			*connector_type = DRM_MODE_CONNECTOR_DVID;
	}

	 
	if ((dev->pdev->device == 0x9498) &&
	    (dev->pdev->subsystem_vendor == 0x1682) &&
	    (dev->pdev->subsystem_device == 0x2452)) {
		struct radeon_device *rdev = dev->dev_private;
		*i2c_bus = radeon_lookup_i2c_gpio(rdev, 0x93);
	}
	return true;
}
