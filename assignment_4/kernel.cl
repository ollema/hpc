__kernel void heat_diff(__global double *cur_temps, __global double *new_temps, float diff_const, int width) {

    // Get the index of the current element
    int i = get_global_id(0);

    // Do the operation
    new_temps[i] = cur_temps[i] * diff_const + width;
}
