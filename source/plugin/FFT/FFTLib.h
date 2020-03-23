#pragma once
#ifndef _FFTLIB_H_
#define _FFTLIB_H_

#include <module/classmacro.h>
#include <FFT/IFFTLib.h>

class CFFTLib : public IFFTLib
{
	X3BEGIN_CLASS_DECLARE(CFFTLib, clsidFFTLib)
		X3DEFINE_INTERFACE_ENTRY(IFFTLib)
		X3END_CLASS_DECLARE()

protected:
	CFFTLib();
	virtual ~CFFTLib();
public:
	complex W[MAX_SAMPLE_N];

	virtual CodeRet_t fft(complex* x, uint32_t size_x);

};

#endif