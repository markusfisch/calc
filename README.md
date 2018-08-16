# calc

A tiny calculator for mathematical expressions in portable C.

## Building

Just run:

	$ make

## Embedding

Drop `calc.c` and `calc.h` into your project and use `calc()` like this:

	double result = 0;
	if (calc("3 + 4", &result, 0)) {
		...
	} else {
		// syntax error
	}
