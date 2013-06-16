/*
 *   O         ,-
 *  ° o    . -´  '     ,-
 *   °  .´        ` . ´,´
 *     ( °   ))     . (
 *      `-;_    . -´ `.`.
 *          `._'       ´
 *
 * 2004 Markus Fisch <mf@markusfisch.de>
 * Public Domain
 */
#include "calc.h"

#include <stdlib.h>
#include <string.h>

/**
 * Calculate a mathematical expression recursively
 *
 * @param expression - a mathematical expression
 * @param result - result
 * @param primacy - determines if the function should return if it enters
 *                  a operation that has no explicit primacy; call with 0
 */
const char *calc( const char *expression, double *result, int primacy )
{
	static const char *primacy_ops = "*/";
	const char *cursor;
	char op = 0;
	double num;

	for( cursor = expression; ; ++expression )
		switch( *expression )
		{
			case ' ':
				continue;
				break;
			case '(':
				{
					/* the following expression is braced
					 * so evaluate it outside this scope */
					if( !(expression = calc( ++expression, &num, 0 )) )
						return NULL;

					/* if there was a minus directly in
					 * front of the braced expression make
					 * the result negative */
					if( *cursor == '-' )
					{
						num = -num;
						++cursor;
					}
				}
				break;
			case '<':
			case '>':
				/* skip second shifting character and
				 * fall through */
				if( *expression != *++expression )
					return NULL;
			case '&':
			case '|':
			case '^':
			case '+':
			case '-':
			case '/':
			case '*':
			case '%':
			case ')':
			case 0:
				if( *cursor != '(' )
					if( cursor == expression )
					{
						/* there is a sequence of stop characters
						 * if cursor and expression are pointing to
						 * the same character which can only be
						 * valid if the stop character is a minus */
						if( *expression != '-' ||
							!strchr( "0123456789()", *(expression+1) ) )
							return NULL;

						/* in which case the character isn't a
						 * operator so don't process it and continue
						 * to the next stop character */
						continue;
					}
					else
					{
						/* get the value before the current
						 * stop character */
						char *stop;

						num = strtod( cursor, &stop );

						/* ignore trailing white space */
						for( ; *stop == ' '; ++stop );

						if( *stop != *expression )
							return NULL;
					}

				switch( op )
				{
					case '+':
					case '-':
						/* the next operation has primacy
						 * over the current operation so
						 * evaluate it first */
						if( *expression &&
							strchr( primacy_ops, *expression ) &&
							!(expression = calc( cursor, &num, 1 )) )
							return NULL;

						switch( op )
						{
							case '+':
								*result += num;
								break;
							case '-':
								*result -= num;
								break;
						}
						break;
					case '/':
						*result /= num;
						break;
					case '*':
						*result *= num;
						break;
					case 0:
						/* the first operation character
						 * was found */
						*result = num;
						break;
				}

				/* move the cursor after the last
				 * stop character and move it over
				 * optional white space */
				for( cursor = expression+1;
					*cursor == ' ';
					++cursor );

				op = *expression;

				if( op && strchr( "&|^<>%", op ) )
				{
#ifndef WIN32
					long long a;
					long long b;
#else
					__int64 a;
					__int64 b;
#endif
					/* if this instance is only called to
					 * calculate a primacy operation return
					 * to the previous instance */
					if( primacy )
						return expression;

					/* logical operations are executed after
					 * all arithmetic operations so evaluate
					 * the rest first */
					if( !(expression = calc( ++expression, &num, 0 )) )
						return NULL;

					/* convert doubles to integers since
					 * logic operations can only be applied
					 * to integers */
#ifndef WIN32
					a = (long long)*result;
					b = (long long)num;
#else
					a = (__int64)*result;
					b = (__int64)num;
#endif

					if( *result != a ||
						num != b )
						return NULL;

					switch( op )
					{
						case '&':
							a &= b;
							break;
						case '|':
							a |= b;
							break;
						case '^':
							a ^= b;
							break;
						case '<':
							a <<= b;
							break;
						case '>':
							a >>= b;
							break;
						case '%':
							a %= b;
							break;
					}

					*result = (double)a;

					return expression;
				}

				/* stop when the end of the string has
				 * been reached or the stop character is a
				 * closing brace or the next operation isn't
				 * (again) a primacy one */
				if( !op ||
					op == ')' ||
					(primacy && !strchr(
						primacy_ops,
						*expression )) )
					return expression;

				break;
		}
}
