#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

int main()
{
    void compute_line( int lines, int degree, int line )
    {
	#define lower_bound 0.000001
	#define upper_bound 10000000000
	
	double complex roots_h[degree];
	int *result_roots = malloc(lines * sizeof(int));
	int *result_iters = malloc(lines * sizeof(int));
	double complex z;
	double old_im_z = 2.0-(4.0*line/lines);
	double distance;
	size_t conv;
	size_t attr;
	double sq_abs_z;
	size_t cx;
	
	switch ( degree )
	{
	    case 1 :
		roots_h[0] = 1.0 + 0.0 * I;
		break;
	    case 2 :
		roots_h[0] = 1.0 + 0.0 * I;
		roots_h[1] = -1.0 + 0.0 * I;
		break;
	    case 3 :
		roots_h[0] = 1.0 + 0.0 * I;
		roots_h[1] = -0.5 -0.86603 * I;
		roots_h[2] = -0.5 + 0.86603 * I;
		break;
	    case 4 :
		roots_h[0] = -1.0 + 0.0 * I;
		roots_h[1] = 1.0 + 0.0 * I;
		roots_h[2] = 0.0 - 1.0 *I;
		roots_h[3] = 0.0 - 1 * I;
		break;
	    case 5 :
		roots_h[0] = 1.0 + 0.0 * I;
		roots_h[1] = -0.80902 - 0.58779 * I;
		roots_h[2] = 0.30902 + 0.95106 * I;
		roots_h[3] = 0.30902 - 0.95106 * I;
		roots_h[4] = -0.80902 + 0.58779 * I;
		break;
	    case 6 :
		roots_h[0] = -1.0 + 0.0 * I;
		roots_h[1] = 1.0 + 0.0 * I;
		roots_h[2] = -0.5 - 0.86603 * I;
		roots_h[3] = 0.5 + 0.86603 * I;
		roots_h[4] = 0.5 - 0.86603 * I;
		roots_h[5] = -0.5 + 0.86603 * I;
		break;
	    case 7 :
		roots_h[0] = 1.0 + 0.0 * I;
		roots_h[1] = -0.90097 - 0.43388 * I;
		roots_h[2] = 0.62349 + 0.78183 * I;
		roots_h[3] = -0.22252 - 0.97493 * I;
		roots_h[4] = -0.22252 + 0.97493 * I;
		roots_h[5] = 0.62349 - 0.78183 * I;
		roots_h[6] = -0.90097 + 0.43388 * I;
		break;
	    case 8 :
		roots_h[0] = -1.0 + 0.0 * I;
		roots_h[1] = 1.0 + 0.0 * I;
		roots_h[2] = 0.0 - 1.0 * I;
		roots_h[3] = 0.0 + 1.0 * I;
		roots_h[4] = -0.70711 - 70711 * I;
		roots_h[5] = 0.70711 + 0.70711 * I;
		roots_h[6] = 0.70711 - 0.70711 * I;
		roots_h[7] = -0.70711 + 0.70711 * I;
		break;
	    case 9 :
		roots_h[0] = 1.0 + 0.0 * I;
		roots_h[1] = -0.93969 - 0.34202 * I;
		roots_h[2] = 0.76604 + 0.64279 * I;
		roots_h[3] = -0.5 - 0.86603 * I;
		roots_h[4] = 0.17365 + 0.98481 * I;
		roots_h[5] = 0.17365 - 0.98481 * I;
		roots_h[6] = 0.5 + 0.86603 * I;
		roots_h[7] = 0.76604 - 0.64279 * I;
		roots_h[8] = -0.93969 + 0.34202 * I;
		break;
	    case 10 :
		roots_h[0] = -1.0 + 0.0 * I;
		roots_h[1] = 1.0 + 0.0 * I;
		roots_h[2] = -0.80902 - 0.58779 * I;
		roots_h[3] = 0.80902 + 0.58779 * I;
		roots_h[4] = -0.30902 - 0.95106 * I;
		roots_h[5] = 0.30902 + 0.95106 * I;
		roots_h[6] = 0.30902 - 0.95106 * I;
		roots_h[7] = -0.30902 + 0.95106 * I;
		roots_h[8] = 0.80902 - 0.58779 * I;
		roots_h[9] = -0.80902 + 0.58779 * I;
		break;
	    default :
		fprintf(stderr,"unexpected degree\n");
		exit(1);
		
	}
	
	for ( double re_z = -2.0; cx < lines; ++cx, re_z += (4.0/lines))
	{
	    z = re_z + old_im_z * I;
	    sq_abs_z = creal(z)*creal(z) + cimag(z)*cimag(z); 
	    for ( conv = 0, attr = -1; ; ++conv )
	    {
		if ( creal(z) > upper_bound || cimag(z) > upper_bound )
		{
		    attr = 0;
		    break;
		}
		if ( sq_abs_z < lower_bound )
		{
		    attr = 0;
		    break;
		}
		if ( conv > 55)
		{
		    attr = 0;
		    break;
		}
		for ( size_t ix = 0; ix < degree; ++ix)
		{
		    distance = ( creal( roots_h[ix] ) - creal(z) ) *
			    ( creal( roots_h[ix] ) - creal(z) ) +
			    ( cimag( roots_h[ix] ) - cimag(z) ) *
			    ( cimag( roots_h[ix] ) - cimag(z) );
		    if ( distance < lower_bound )
		    {
			attr = ix + 1;
			break;
		    }
		}
		if ( attr != -1 )
		{
		    break;
		}
		switch ( degree )
		{
		    case 1 :
			z = -1.0;
			break;
		    case 2 :
			z = z*(1.0/2) + 1.0/(2.0*z);
			break;
		    case 3 :
			z = z*(2.0/3) + 1.0/(3*z*z);
			break;
		    case 4 :
			z = z*(3.0/4) + 1.0/(4*z*z*z);
			break;
		    case 5 :
			z = z*(4.0/5) + 1.0/(5*z*z*z*z);
			break;
		    case 6 :
			z = z*(5.0/6) + 1.0/(6*z*z*z*z*z);
			break;
		    case 7 :
			z = z*(6.0/7) + 1.0/(7*z*z*z*z*z*z);
			break;
		    case 8 :
			z = z*(7.0/8) + 1.0/(8*z*z*z*z*z*z*z);
			break;
		    case 9 :
			z = z*(8.0/9) + 1.0/(9*z*z*z*z*z*z*z*z);
			break;
		    case 10 :
			z = z*(9.0/10) + 1.0/(10*z*z*z*z*z*z*z*z*z);
			break; 
		    default:
			fprintf(stderr, "unexpected degree\n");
			exit(1);
		}
		if ( attr == 0 )
		{
		    result_roots[cx] = attr;
		}
		else
		{
		    result_roots[cx] = attr + 1; 
		}
		result_iters[cx] = conv;
	    }
		    
	}
    }    
}
