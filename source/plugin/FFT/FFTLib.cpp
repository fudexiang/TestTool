#include <module/plugininc.h>		/* important */
#include "FFTLib.h"
#include "math.h"

CFFTLib::CFFTLib()
{
}

CFFTLib::~CFFTLib()
{

}

static void assign(complex* dst, complex* src)
{
	dst->real = src->real;
	dst->img = src->img;
}

static void add(complex a, complex b, complex* c)
{
	c->real = a.real + b.real;
	c->img = a.img + b.img;
}

static void mul(complex a, complex b, complex* c)
{
	c->real = a.real * b.real - a.img * b.img;
	c->img = a.real * b.img + a.img * b.real;
}

static void sub(complex a, complex b, complex* c)
{
	c->real = a.real - b.real;
	c->img = a.img - b.img;
}

static void change(complex* x, uint32_t size_x)
{
	complex temp;
	unsigned short i = 0, j = 0, k = 0;
	double t;
	for (i = 0; i < size_x; i++)
	{
		k = i; j = 0;
		t = (log(size_x) / log(2));
		while ((t--) > 0)    //利用按位与以及循环实现码位颠倒  
		{
			j = j << 1;
			j |= (k & 1);
			k = k >> 1;
		}
		if (j > i)    //将x(n)的码位互换  
		{

			assign(&temp, &x[i]);
			assign(&x[i], &x[j]);
			assign(&x[j], &temp);

		}
	}
}

static void initW(complex* W,uint32_t size_x)
{
	int i;
	for (i = 0; i < size_x; i++)
	{
		W[i].real = cos(2 * PI / size_x * i);
		W[i].img = -1 * sin(2 * PI / size_x * i);
	}
}

CodeRet_t CFFTLib::fft(complex *x,uint32_t size_x)
{
	int i = 0, j = 0, k = 0, l = 0;
	complex up, down, product;

	initW(W, size_x);

	change(x,size_x); 
	for (i = 0; i < log(size_x) / log(2); i++)        /*一级蝶形运算 stage */
	{
		l = 1 << i;
		for (j = 0; j < size_x; j += 2 * l)     /*一组蝶形运算 group,每个group的蝶形因子乘数不同*/
		{
			for (k = 0; k < l; k++)        /*一个蝶形运算 每个group内的蝶形运算*/
			{
				mul(x[j + k + l], W[size_x * k / 2 / l], &product);
				add(x[j + k], product, &up);
				sub(x[j + k], product, &down);

				assign(&x[j + k], &up);
				assign(&x[j + k + l], &down);

			}
		}
	}

	return RET_OK;
}

