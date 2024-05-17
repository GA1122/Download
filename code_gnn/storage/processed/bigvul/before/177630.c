void reference_dct_1d(double input[8], double output[8]) {
 const double kPi = 3.141592653589793238462643383279502884;
 const double kInvSqrt2 = 0.707106781186547524400844362104;
 for (int k = 0; k < 8; k++) {
    output[k] = 0.0;
 for (int n = 0; n < 8; n++)
      output[k] += input[n]*cos(kPi*(2*n+1)*k/16.0);
 if (k == 0)
      output[k] = output[k]*kInvSqrt2;
 }
}
