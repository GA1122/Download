bool radeon_atom_get_clock_info(struct drm_device *dev)
{
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	int index = GetIndexIntoMasterTable(DATA, FirmwareInfo);
	union firmware_info *firmware_info;
	uint8_t frev, crev;
	struct radeon_pll *p1pll = &rdev->clock.p1pll;
	struct radeon_pll *p2pll = &rdev->clock.p2pll;
	struct radeon_pll *dcpll = &rdev->clock.dcpll;
	struct radeon_pll *spll = &rdev->clock.spll;
	struct radeon_pll *mpll = &rdev->clock.mpll;
	uint16_t data_offset;

	if (atom_parse_data_header(mode_info->atom_context, index, NULL,
				   &frev, &crev, &data_offset)) {
		firmware_info =
			(union firmware_info *)(mode_info->atom_context->bios +
						data_offset);
		 
		p1pll->reference_freq =
		    le16_to_cpu(firmware_info->info.usReferenceClock);
		p1pll->reference_div = 0;

		if (crev < 2)
			p1pll->pll_out_min =
				le16_to_cpu(firmware_info->info.usMinPixelClockPLL_Output);
		else
			p1pll->pll_out_min =
				le32_to_cpu(firmware_info->info_12.ulMinPixelClockPLL_Output);
		p1pll->pll_out_max =
		    le32_to_cpu(firmware_info->info.ulMaxPixelClockPLL_Output);

		if (crev >= 4) {
			p1pll->lcd_pll_out_min =
				le16_to_cpu(firmware_info->info_14.usLcdMinPixelClockPLL_Output) * 100;
			if (p1pll->lcd_pll_out_min == 0)
				p1pll->lcd_pll_out_min = p1pll->pll_out_min;
			p1pll->lcd_pll_out_max =
				le16_to_cpu(firmware_info->info_14.usLcdMaxPixelClockPLL_Output) * 100;
			if (p1pll->lcd_pll_out_max == 0)
				p1pll->lcd_pll_out_max = p1pll->pll_out_max;
		} else {
			p1pll->lcd_pll_out_min = p1pll->pll_out_min;
			p1pll->lcd_pll_out_max = p1pll->pll_out_max;
		}

		if (p1pll->pll_out_min == 0) {
			if (ASIC_IS_AVIVO(rdev))
				p1pll->pll_out_min = 64800;
			else
				p1pll->pll_out_min = 20000;
		} else if (p1pll->pll_out_min > 64800) {
			 
			if (!radeon_new_pll)
				p1pll->pll_out_min = 64800;
		}

		p1pll->pll_in_min =
		    le16_to_cpu(firmware_info->info.usMinPixelClockPLL_Input);
		p1pll->pll_in_max =
		    le16_to_cpu(firmware_info->info.usMaxPixelClockPLL_Input);

		*p2pll = *p1pll;

		 
		spll->reference_freq =
		    le16_to_cpu(firmware_info->info.usReferenceClock);
		spll->reference_div = 0;

		spll->pll_out_min =
		    le16_to_cpu(firmware_info->info.usMinEngineClockPLL_Output);
		spll->pll_out_max =
		    le32_to_cpu(firmware_info->info.ulMaxEngineClockPLL_Output);

		 
		if (spll->pll_out_min == 0) {
			if (ASIC_IS_AVIVO(rdev))
				spll->pll_out_min = 64800;
			else
				spll->pll_out_min = 20000;
		}

		spll->pll_in_min =
		    le16_to_cpu(firmware_info->info.usMinEngineClockPLL_Input);
		spll->pll_in_max =
		    le16_to_cpu(firmware_info->info.usMaxEngineClockPLL_Input);

		 
		mpll->reference_freq =
		    le16_to_cpu(firmware_info->info.usReferenceClock);
		mpll->reference_div = 0;

		mpll->pll_out_min =
		    le16_to_cpu(firmware_info->info.usMinMemoryClockPLL_Output);
		mpll->pll_out_max =
		    le32_to_cpu(firmware_info->info.ulMaxMemoryClockPLL_Output);

		 
		if (mpll->pll_out_min == 0) {
			if (ASIC_IS_AVIVO(rdev))
				mpll->pll_out_min = 64800;
			else
				mpll->pll_out_min = 20000;
		}

		mpll->pll_in_min =
		    le16_to_cpu(firmware_info->info.usMinMemoryClockPLL_Input);
		mpll->pll_in_max =
		    le16_to_cpu(firmware_info->info.usMaxMemoryClockPLL_Input);

		rdev->clock.default_sclk =
		    le32_to_cpu(firmware_info->info.ulDefaultEngineClock);
		rdev->clock.default_mclk =
		    le32_to_cpu(firmware_info->info.ulDefaultMemoryClock);

		if (ASIC_IS_DCE4(rdev)) {
			rdev->clock.default_dispclk =
				le32_to_cpu(firmware_info->info_21.ulDefaultDispEngineClkFreq);
			if (rdev->clock.default_dispclk == 0)
				rdev->clock.default_dispclk = 60000;  
			rdev->clock.dp_extclk =
				le16_to_cpu(firmware_info->info_21.usUniphyDPModeExtClkFreq);
		}
		*dcpll = *p1pll;

		return true;
	}

	return false;
}
