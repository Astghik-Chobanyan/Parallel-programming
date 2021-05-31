/*
__kernel void hello(__global char* string)
{
string[0] = 'H';
string[1] = 'e';
string[2] = 'l';
string[3] = 'l';
string[4] = 'o';
string[5] = ',';
string[6] = ' ';
string[7] = 'W';
string[8] = 'o';
string[9] = 'r';
string[10] = 'l';
string[11] = 'd';
string[12] = '!';
string[13] = '!';
string[14] = '!';
string[15] = '!';
string[16] = '!';
string[17] = '\0';

}
*/


__kernel void getMatrixSum(__global const float *matrix,
                            __global const float *vector,
                            __global float* outputVector,
                                            ulong n){

        
        ulong num=(ulong)get_global_id(0);
        float sum=0;
        //printf(" sum is=-----------==%u\n",sum);
        if(num<n){
            for(ulong i=0;i<n;i++)
            {
                sum+=matrix[num*n+i]*vector[i];
                //printf(" sum is===%u\n",sum);
            }
            printf(" sum is=-----------==%u\n",sum);
            outputVector[num]=sum;
        }


}