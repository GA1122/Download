void butterfly_16x16_dct_1d(double input[16], double output[16]) {
 double step[16];
 double intermediate[16];
 double temp1, temp2;

  step[ 0] = input[0] + input[15];
  step[ 1] = input[1] + input[14];
  step[ 2] = input[2] + input[13];
  step[ 3] = input[3] + input[12];
  step[ 4] = input[4] + input[11];
  step[ 5] = input[5] + input[10];
  step[ 6] = input[6] + input[ 9];
  step[ 7] = input[7] + input[ 8];
  step[ 8] = input[7] - input[ 8];
  step[ 9] = input[6] - input[ 9];
  step[10] = input[5] - input[10];
  step[11] = input[4] - input[11];
  step[12] = input[3] - input[12];
  step[13] = input[2] - input[13];
  step[14] = input[1] - input[14];
  step[15] = input[0] - input[15];

  output[0] = step[0] + step[7];
  output[1] = step[1] + step[6];
  output[2] = step[2] + step[5];
  output[3] = step[3] + step[4];
  output[4] = step[3] - step[4];
  output[5] = step[2] - step[5];
  output[6] = step[1] - step[6];
  output[7] = step[0] - step[7];

  temp1 = step[ 8] * C7;
  temp2 = step[15] * C9;
  output[ 8] = temp1 + temp2;

  temp1 = step[ 9] * C11;
  temp2 = step[14] * C5;
  output[ 9] = temp1 - temp2;

  temp1 = step[10] * C3;
  temp2 = step[13] * C13;
  output[10] = temp1 + temp2;

  temp1 = step[11] * C15;
  temp2 = step[12] * C1;
  output[11] = temp1 - temp2;

  temp1 = step[11] * C1;
  temp2 = step[12] * C15;
  output[12] = temp2 + temp1;

  temp1 = step[10] * C13;
  temp2 = step[13] * C3;
  output[13] = temp2 - temp1;

  temp1 = step[ 9] * C5;
  temp2 = step[14] * C11;
  output[14] = temp2 + temp1;

  temp1 = step[ 8] * C9;
  temp2 = step[15] * C7;
  output[15] = temp2 - temp1;

  step[ 0] = output[0] + output[3];
  step[ 1] = output[1] + output[2];
  step[ 2] = output[1] - output[2];
  step[ 3] = output[0] - output[3];

  temp1 = output[4] * C14;
  temp2 = output[7] * C2;
  step[ 4] = temp1 + temp2;

  temp1 = output[5] * C10;
  temp2 = output[6] * C6;
  step[ 5] = temp1 + temp2;

  temp1 = output[5] * C6;
  temp2 = output[6] * C10;
  step[ 6] = temp2 - temp1;

  temp1 = output[4] * C2;
  temp2 = output[7] * C14;
  step[ 7] = temp2 - temp1;

  step[ 8] = output[ 8] + output[11];
  step[ 9] = output[ 9] + output[10];
  step[10] = output[ 9] - output[10];
  step[11] = output[ 8] - output[11];

  step[12] = output[12] + output[15];
  step[13] = output[13] + output[14];
  step[14] = output[13] - output[14];
  step[15] = output[12] - output[15];

  output[ 0] = (step[ 0] + step[ 1]);
  output[ 8] = (step[ 0] - step[ 1]);

  temp1 = step[2] * C12;
  temp2 = step[3] * C4;
  temp1 = temp1 + temp2;
  output[ 4] = 2*(temp1 * C8);

  temp1 = step[2] * C4;
  temp2 = step[3] * C12;
  temp1 = temp2 - temp1;
  output[12] = 2 * (temp1 * C8);

  output[ 2] = 2 * ((step[4] + step[ 5]) * C8);
  output[14] = 2 * ((step[7] - step[ 6]) * C8);

  temp1 = step[4] - step[5];
  temp2 = step[6] + step[7];
  output[ 6] = (temp1 + temp2);
  output[10] = (temp1 - temp2);

  intermediate[8] = step[8] + step[14];
  intermediate[9] = step[9] + step[15];

  temp1 = intermediate[8] * C12;
  temp2 = intermediate[9] * C4;
  temp1 = temp1 - temp2;
  output[3] = 2 * (temp1 * C8);

  temp1 = intermediate[8] * C4;
  temp2 = intermediate[9] * C12;
  temp1 = temp2 + temp1;
  output[13] = 2 * (temp1 * C8);

  output[ 9] = 2 * ((step[10] + step[11]) * C8);

  intermediate[11] = step[10] - step[11];
  intermediate[12] = step[12] + step[13];
  intermediate[13] = step[12] - step[13];
  intermediate[14] = step[ 8] - step[14];
  intermediate[15] = step[ 9] - step[15];

  output[15] = (intermediate[11] + intermediate[12]);
  output[ 1] = -(intermediate[11] - intermediate[12]);

  output[ 7] = 2 * (intermediate[13] * C8);

  temp1 = intermediate[14] * C12;
  temp2 = intermediate[15] * C4;
  temp1 = temp1 - temp2;
  output[11] = -2 * (temp1 * C8);

  temp1 = intermediate[14] * C4;
  temp2 = intermediate[15] * C12;
  temp1 = temp2 + temp1;
  output[ 5] = 2 * (temp1 * C8);
}
