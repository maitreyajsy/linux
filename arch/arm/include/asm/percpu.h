/*
 * Copyright 2012 Calxeda, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _ASM_ARM_PERCPU_H_
#define _ASM_ARM_PERCPU_H_

/*
 * Same as asm-generic/percpu.h, except that we store the per cpu offset
 * in the TPIDRPRW. TPIDRPRW only exists on V6K and V7
 */
#if defined(CONFIG_SMP) && !defined(CONFIG_CPU_V6)
static inline void set_my_cpu_offset(unsigned long off)
{
	/* Set TPIDRPRW */
	asm volatile("mcr p15, 0, %0, c13, c0, 4" : : "r" (off) : "memory");
	/* [lksq:20150704] thread and process id register TPIDR
	     4 값이 설정될 경우 write privileged only thread and process ID register
		 arm instrument 공식 사이트 참조

         권한 설정하는 내용으로 off = 0 이면 이것이 왜 clear랑 관계있는 것일까 ??
		 set_my_cpu_offset(0)는 clear __my_cpu_offset on boot cpu
		 __my_cpu_offset 은 반대로 read MRC 로 되어있음
	 */

}

static inline unsigned long __my_cpu_offset(void)
{
	unsigned long off;
	register unsigned long *sp asm ("sp");

	/*
	 * Read TPIDRPRW.
	 * We want to allow caching the value, so avoid using volatile and
	 * instead use a fake stack read to hazard against barrier().
	 */
	asm("mrc p15, 0, %0, c13, c0, 4" : "=r" (off) : "Q" (*sp));

	return off;
}
#define __my_cpu_offset __my_cpu_offset()
#else
#define set_my_cpu_offset(x)	do {} while(0)

#endif /* CONFIG_SMP */

#include <asm-generic/percpu.h>

#endif /* _ASM_ARM_PERCPU_H_ */
