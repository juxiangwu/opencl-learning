/**
*@bref Vector add
*@author Jenson Wu
*/

__kernel void vector_add(__global const int * vec1,__global const int * vec2,__global int * out){
	//get global index id
	int gid = get_global_id(0);
	
	out[gid] = vec1[gid] + vec2[gid];
	
}
