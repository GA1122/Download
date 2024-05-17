 int SetLayerId(int frame_num, int num_temp_layers) {
 int layer_id = 0;
 if (num_temp_layers == 2) {
 if (frame_num % 2 == 0) {
        layer_id = 0;
 } else {
        layer_id = 1;
 }
 } else if (num_temp_layers == 3) {
 if (frame_num % 4 == 0) {
        layer_id = 0;
 } else if ((frame_num - 2) % 4 == 0) {
        layer_id = 1;
 } else if ((frame_num - 1) % 2 == 0) {
        layer_id = 2;
 }
 }
 return layer_id;
 }
