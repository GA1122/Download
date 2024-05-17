 int main(int argc, char **argv)
 {
 	int fmtid;
 	int id;
 	char *infile;
 	jas_stream_t *instream;
 	jas_image_t *image;
 	int width;
 	int height;
 	int depth;
  	int numcmpts;
  	int verbose;
  	char *fmtname;
// 	int debug;
  
  	if (jas_init()) {
  		abort();
 	}
 
 	cmdname = argv[0];
  
  	infile = 0;
  	verbose = 0;
// 	debug = 0;
  
  	 
  	while ((id = jas_getopt(argc, argv, opts)) >= 0) {
 		switch (id) {
 		case OPT_VERBOSE:
 			verbose = 1;
 			break;
 		case OPT_VERSION:
  			printf("%s\n", JAS_VERSION);
  			exit(EXIT_SUCCESS);
  			break;
// 		case OPT_DEBUG:
// 			debug = atoi(jas_optarg);
// 			break;
  		case OPT_INFILE:
  			infile = jas_optarg;
  			break;
 		case OPT_HELP:
 		default:
 			usage();
 			break;
  		}
  	}
  
// 	jas_setdbglevel(debug);
// 
  	 
  	if (infile) {
  		 
 		if (!(instream = jas_stream_fopen(infile, "rb"))) {
 			fprintf(stderr, "cannot open input image file %s\n", infile);
 			exit(EXIT_FAILURE);
 		}
 	} else {
 		 
 		if (!(instream = jas_stream_fdopen(0, "rb"))) {
 			fprintf(stderr, "cannot open standard input\n");
 			exit(EXIT_FAILURE);
 		}
 	}
 
 	if ((fmtid = jas_image_getfmt(instream)) < 0) {
 		fprintf(stderr, "unknown image format\n");
 	}
  
  	 
  	if (!(image = jas_image_decode(instream, fmtid, 0))) {
// 		jas_stream_close(instream);
  		fprintf(stderr, "cannot load image\n");
  		return EXIT_FAILURE;
  	}
 
 	 
 	jas_stream_close(instream);
 
 	numcmpts = jas_image_numcmpts(image);
 	width = jas_image_cmptwidth(image, 0);
 	height = jas_image_cmptheight(image, 0);
 	depth = jas_image_cmptprec(image, 0);
 	if (!(fmtname = jas_image_fmttostr(fmtid))) {
 		abort();
 	}
 	printf("%s %d %d %d %d %ld\n", fmtname, numcmpts, width, height, depth, (long) jas_image_rawsize(image));
 
 	jas_image_destroy(image);
 	jas_image_clearfmts();
 
 	return EXIT_SUCCESS;
 }