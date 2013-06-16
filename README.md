calc
====

A tiny calculator for mathematical expressions in portable C.

Sample
------

Use calc() like this:

	double result = 0;

	if( !calc( "3+4", &result, 0 ) )
		printf( "syntax error\n" );
	else
		printf( "result: %f\n", result );
