static gdFontPtr php_find_gd_font(int size TSRMLS_DC)
{
	gdFontPtr font;
	int ind_type;

	switch (size) {
		case 1:
			 font = gdFontTiny;
			 break;
		case 2:
			 font = gdFontSmall;
			 break;
		case 3:
			 font = gdFontMediumBold;
			 break;
		case 4:
			 font = gdFontLarge;
			 break;
		case 5:
			 font = gdFontGiant;
			 break;
		default:
			font = zend_list_find(size - 5, &ind_type);
			 if (!font || ind_type != le_gd_font) {
				  if (size < 1) {
					   font = gdFontTiny;
				  } else {
					   font = gdFontGiant;
				  }
			 }
			 break;
	}

	return font;
}