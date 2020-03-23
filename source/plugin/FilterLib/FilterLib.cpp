#include <module/plugininc.h>		/* important */
#include "FilterLib.h"
#include "math.h"

CFilterLib::CFilterLib()
{

}

CFilterLib::~CFilterLib()
{

}

void CFilterLib::Init_Filter(FILTER_TYPE_t type,double ts, double freq_center, double q)
{
	double omega = 2 * PI * freq_center * ts;
	double  sine = sin(omega);
	double  cosine = cos(omega);
	double  alpha = sine / (2 * q);

	LPFILTERINFO fi = &pre_filter_Vo;

	memset(&pre_filter_Vo, 0, sizeof(FILTERINFO));

	pre_filter_Vo.type.type = type;
	pre_filter_Vo.ts = ts;
	pre_filter_Vo.type.fl = freq_center;	// 中心频率为1Hz
	pre_filter_Vo.q = q;					// 品质因子为1，对中心频率信号滤波效果最佳

	fi->b[0] = fi->b[1] = fi->b[2] = fi->a[0] = fi->a[1] = fi->a[2] = 0;

	switch (fi->type.type)
	{
	case FILTER_1ST_LP:
	case FILTER_1ST_HP:
		fi->b[0] = fi->type.fl * 2.0 * PI * fi->ts;
		fi->a[1] = 1 - fi->b[0];
		break;
	case FILTER_2ND_LP:
		fi->a[0] = 1 + alpha;
		fi->a[1] = (-2 * cosine) / fi->a[0];
		fi->a[2] = (1 - alpha) / fi->a[0];
		fi->b[0] = ((1 - cosine) / 2) / fi->a[0];
		fi->b[1] = (1 - cosine) / fi->a[0];
		fi->b[2] = ((1 - cosine) / 2) / fi->a[0];
		break;
	case FILTER_2ND_HP:
		/*
		y(n) = (b0 * x(n) + b1*x(n-1) + b2*x(n-2) - a1*y(n-1) -a2*y(n-2))/a0
		*/
		fi->a[0] = 1 + alpha;
		fi->a[1] = (-2 * cosine) / fi->a[0];
		fi->a[2] = (1 - alpha) / fi->a[0];
		fi->b[0] = ((1 + cosine) / 2) / fi->a[0];
		fi->b[1] = (-(1 + cosine)) / fi->a[0];
		fi->b[2] = ((1 + cosine) / 2) / fi->a[0];

		//fi->b[0] * fi->x[0] + fi->b[1] * fi->x[1] + fi->b[2] * fi->x[2] - fi->a[1] * fi->y[0] - fi->a[2] * fi->y[1]
		/*
		printf("===>>>y(n) = %f * x(0) + %f * x(1) + %f * x(2) -%f * y(0) -%f * y(1)\r\n", \
			fi->b[0], fi->b[1], fi->b[2], \
			fi->a[1], fi->a[2]);
		*/
		break;
	case FILTER_2ND_BP:
	case FILTER_2ND_BS:
		fi->a[0] = 1 + alpha;
		fi->a[1] = (-2 * cosine) / fi->a[0];
		fi->a[2] = (1 - alpha) / fi->a[0];
		fi->b[0] = (sine / 2) / fi->a[0];
		fi->b[1] = 0;
		fi->b[2] = (-sine / 2) / fi->a[0];
		break;
	}

	fi->x[0] = fi->x[1] = fi->x[2] = fi->y[0] = fi->y[1] = 0;
}

double CFilterLib::Filter(double in)
{
	double f = in;
	LPFILTERINFO fi = &pre_filter_Vo;
	if (fi->type.type == FILTER_1ST_LP || fi->type.type == FILTER_1ST_HP)
	{
		f = fi->b[0] * in + fi->a[1] * fi->y[0];
		fi->y[0] = f;
	}
	else if (fi->type.type)
	{
		fi->x[2] = fi->x[1];
		fi->x[1] = fi->x[0];
		fi->x[0] = in;
		f = fi->b[0] * fi->x[0] + fi->b[1] * fi->x[1] + fi->b[2] * fi->x[2] - fi->a[1] * fi->y[0] - fi->a[2] * fi->y[1];
		fi->y[1] = fi->y[0];
		fi->y[0] = f;
	}

	return f;
}
