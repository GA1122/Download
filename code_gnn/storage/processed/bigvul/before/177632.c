void reference_idct_1d(double input[8], double output[8]) {
 const double kPi = 3.141592653589793238462643383279502884;
 const double kSqrt2 = 1.414213562373095048801688724209698;
 for (int k = 0; k < 8; k++) {
    output[k] = 0.0;
 for (int n = 0; n < 8; n++) {
      output[k] += input[n]*cos(kPi*(2*k+1)*n/16.0);
 if (n == 0)
        output[k] = output[k]/kSqrt2;
 }
 }
}
