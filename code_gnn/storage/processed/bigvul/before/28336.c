static int au1200_setlocation (struct au1200fb_device *fbdev, int plane,
	int xpos, int ypos)
{
	uint32 winctrl0, winctrl1, winenable, fb_offset = 0;
	int xsz, ysz;

	 

	winctrl0 = lcd->window[plane].winctrl0;
	winctrl1 = lcd->window[plane].winctrl1;
	winctrl0 &= (LCD_WINCTRL0_A | LCD_WINCTRL0_AEN);
	winctrl1 &= ~(LCD_WINCTRL1_SZX | LCD_WINCTRL1_SZY);

	 
	xsz = win->w[plane].xres;
	ysz = win->w[plane].yres;
	if ((xpos + win->w[plane].xres) > panel->Xres) {
		 
		xsz = panel->Xres - xpos;  
		 
	}

	if ((ypos + win->w[plane].yres) > panel->Yres) {
		 
		ysz = panel->Yres - ypos;  
		 
	}

	if (xpos < 0) {
		 
		xsz = win->w[plane].xres + xpos;
		fb_offset += (((0 - xpos) * winbpp(lcd->window[plane].winctrl1))/8);
		xpos = 0;
		 
	}

	if (ypos < 0) {
		 
		ysz = win->w[plane].yres + ypos;
		 
		ypos = 0;
		 
	}

	 
	win->w[plane].xpos = xpos;
	win->w[plane].ypos = ypos;

	xsz -= 1;
	ysz -= 1;
	winctrl0 |= (xpos << 21);
	winctrl0 |= (ypos << 10);
	winctrl1 |= (xsz << 11);
	winctrl1 |= (ysz << 0);

	 
	winenable = lcd->winenable & (1 << plane);
	au_sync();
	lcd->winenable &= ~(1 << plane);
	lcd->window[plane].winctrl0 = winctrl0;
	lcd->window[plane].winctrl1 = winctrl1;
	lcd->window[plane].winbuf0 =
	lcd->window[plane].winbuf1 = fbdev->fb_phys;
	lcd->window[plane].winbufctrl = 0;  
	lcd->winenable |= winenable;
	au_sync();

	return 0;
}
