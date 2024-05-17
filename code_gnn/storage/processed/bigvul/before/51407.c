static int clip_1d(int *x0, int *y0, int *x1, int *y1, int maxdim) {
	double m;       

	if (*x0 < 0) {   
		if(*x1 < 0) {  
			return 0;
		}
		m = (*y1 - *y0)/(double)(*x1 - *x0);  
		 
		*y0 -= (int)(m * *x0);
		*x0 = 0;
		 
		if (*x1 > maxdim) {
			*y1 += (int)(m * (maxdim - *x1));
			*x1 = maxdim;
		}
		return 1;
	}
	if (*x0 > maxdim) {  
		if (*x1 > maxdim) {  
			return 0;
		}
		m = (*y1 - *y0)/(double)(*x1 - *x0);   
		*y0 += (int)(m * (maxdim - *x0));  
		*x0 = maxdim;
		 
		if (*x1 < 0) {
			*y1 -= (int)(m * *x1);
			*x1 = 0;
		}
		return 1;
	}
	 
	if (*x1 > maxdim) {  
		m = (*y1 - *y0)/(double)(*x1 - *x0);   
		*y1 += (int)(m * (maxdim - *x1));
		*x1 = maxdim;
		return 1;
	}
	if (*x1 < 0) {  
		m = (*y1 - *y0)/(double)(*x1 - *x0);   
		*y1 -= (int)(m * *x1);
		*x1 = 0;
		return 1;
	}
	 
	return 1;
}
