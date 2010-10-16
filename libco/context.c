/* Copyright (c) 2005-2006 Russ Cox, MIT; see COPYRIGHT */

#include "power-ucontext.h"
#include <stdarg.h>

void
makecontext(ucontext_t *ucp, void (*func)(void), int argc, ...)
{
	ulong *sp, *tos;
	va_list arg;

	tos = (ulong*)ucp->uc_stack.ss_sp+ucp->uc_stack.ss_size/sizeof(ulong);
	sp = tos - 16;	
	ucp->mc.pc = (uint32_t)func;
	ucp->mc.sp = (uint32_t)sp;
	va_start(arg, argc);
	ucp->mc.r3 = va_arg(arg, long);
	va_end(arg);
}

int
swapcontext(ucontext_t *oucp, const ucontext_t *ucp)
{
	if(getcontext(oucp) == 0)
		setcontext(ucp);
	return 0;
}

