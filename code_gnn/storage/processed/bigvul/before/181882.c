  jas_matrix_t *jas_seq2d_input(FILE *in)
  {
  	jas_matrix_t *matrix;
	int i;
	int j;
// 	jas_matind_t i;
// 	jas_matind_t j;
  	long x;
	int numrows;
	int numcols;
	int xoff;
	int yoff;
	if (fscanf(in, "%d %d", &xoff, &yoff) != 2)
// 	jas_matind_t numrows;
// 	jas_matind_t numcols;
// 	jas_matind_t xoff;
// 	jas_matind_t yoff;
// 	long tmp_xoff;
// 	long tmp_yoff;
// 	long tmp_numrows;
// 	long tmp_numcols;
// 
// 	if (fscanf(in, "%ld %ld", &tmp_xoff, &tmp_yoff) != 2) {
  		return 0;
	if (fscanf(in, "%d %d", &numcols, &numrows) != 2)
// 	}
// 	xoff = tmp_xoff;
// 	yoff = tmp_yoff;
// 	if (fscanf(in, "%ld %ld", &tmp_numcols, &tmp_numrows) != 2) {
  		return 0;
	if (!(matrix = jas_seq2d_create(xoff, yoff, xoff + numcols, yoff + numrows)))
// 	}
// 	numrows = tmp_numrows;
// 	numcols = tmp_numcols;
// 	if (!(matrix = jas_seq2d_create(xoff, yoff, xoff + numcols,
// 	  yoff + numrows))) {
  		return 0;
// 	}
  
  	if (jas_matrix_numrows(matrix) != numrows ||
  	  jas_matrix_numcols(matrix) != numcols) {
 		abort();
 	}
 
 	 
 	for (i = 0; i < jas_matrix_numrows(matrix); i++) {
 		for (j = 0; j < jas_matrix_numcols(matrix); j++) {
 			if (fscanf(in, "%ld", &x) != 1) {
 				jas_matrix_destroy(matrix);
 				return 0;
 			}
 			jas_matrix_set(matrix, i, j, JAS_CAST(jas_seqent_t, x));
 		}
 	}
 
 	return matrix;
 }