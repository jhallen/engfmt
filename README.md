# engfmt

Here is a simple library for converting ASCII floating point numbers
(perhaps the result from sprintf) into "engineering format"- so that the
exponent is a multiple of 3.

Also included is a function to insert commas between every three digits.

## Interface

    extern char *engfmt(char *d, size_t d_len, const char *s);

	's' is pointer to a zero-terimnated ASCII floating point number.

	'd'/'d_len' contains pointer and size of a buffer for the result.

	Returns 'd'.

    extern char *insert_commas(char *dst, const char *src, char sep);

	'src' is pointer to zero-terminated ASCII decimal or floating
	pointer number.

	'dst' is pointer to a buffer.

	'sep' is separator character to insert, typically ',' or '_'.

	Returns 'dst'.

## Try it

Here is a simple demo main.c:

        char buf[80];
        char buf1[80];
        printf("%s\n", engfmt(buf, sizeof(buf), argv[1]));
        printf("%s\n", insert_commas(buf1, buf, ','));

Here is the result:

	laptop:~/engfmt$ make
	cc    -c -o engfmt.o engfmt.c
	cc    -c -o main.o main.c
	cc -o engfmt engfmt.o main.o

        laptop:~/$ ./engfmt 1.2345
        1.2345
        1.234,5
        laptop:~/engfmt$ ./engfmt 1.2345e0
        1.2345
        1.234,5
        laptop:~/engfmt$ ./engfmt 1.2345e1
        12.345
        12.345
        laptop:~/engfmt$ ./engfmt 1.2345e2
        123.45
        123.45
        laptop:~/engfmt$ ./engfmt 1.2345e3
        1.2345e3
        1.234,5e3
        laptop:~/engfmt$ ./engfmt 1.2345e4
        12.345e3
        12.345e3
        laptop:~/engfmt$ ./engfmt 1.2345e5
        123.45e3
        123.45e3
        laptop:~/engfmt$ ./engfmt 1.2345e6
        1.2345e6
        1.234,5e6
        laptop:~/engfmt$ ./engfmt 1.2345e7
        12.345e6
        12.345e6
        laptop:~/engfmt$ ./engfmt 1.2345e-1
        123.45e-3
        123.45e-3
        laptop:~/engfmt$ ./engfmt 1.2345e-2
        12.345e-3
        12.345e-3
        laptop:~/engfmt$ ./engfmt 1.2345e-3
        1.2345e-3
        1.234,5e-3
        laptop:~/engfmt$ ./engfmt 1.2345e-4
        123.45e-6
        123.45e-6
        laptop:~/engfmt$ ./engfmt 1.2345e-5
        12.345e-6
        12.345e-6
        laptop:~/engfmt$ ./engfmt 1.2345e-6
        1.2345e-6
        1.234,5e-6
        laptop:~/engfmt$ ./engfmt 1.2345e-7
        123.45e-9
        123.45e-9
