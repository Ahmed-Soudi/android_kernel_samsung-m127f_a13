/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PGTABLE_H
#define _LINUX_PGTABLE_H

/*
 * Compatibility wrapper for newer in-tree code on this Samsung 4.19 kernel.
 *
 * Newer kernels provide <linux/pgtable.h> as a generic page-table wrapper.
 * Samsung's 4.19 tree predates that header, while <linux/mm.h> already:
 *
 *   - includes <asm/pgtable.h>
 *   - provides the generic MM/page-table declarations
 *   - provides the fallback untagged_addr() definition
 *
 * KernelSU-Next v3.3.0 includes <linux/pgtable.h> from sucompat.c.
 * Forwarding to the existing Samsung 4.19 MM interface keeps that source
 * compatible without importing a newer kernel's page-table implementation.
 */

#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/version.h>

/*
 * KernelSU-Next v3.3.0 also expects strncpy_from_user_nofault().
 *
 * This helper is not available in Samsung's 4.19 tree.  Reproduce the
 * required no-fault behaviour using the existing 4.19 uaccess primitives.
 *
 * pagefault_disable() prevents the copy from resolving user faults while
 * strncpy_from_user() preserves the normal user-address validation and
 * string-copy semantics expected by KernelSU.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 8, 0)
#ifndef KSU_HAVE_STRNCPY_FROM_USER_NOFAULT
static inline long
ksu_strncpy_from_user_nofault(char *dst,
			      const void __user *unsafe_addr,
			      long count)
{
	long ret;

	if (unlikely(count <= 0))
		return 0;

	pagefault_disable();

	ret = strncpy_from_user(dst,
			       (const char __user *)unsafe_addr,
			       count);

	pagefault_enable();

	return ret;
}

#define strncpy_from_user_nofault ksu_strncpy_from_user_nofault
#endif
#endif

#endif /* _LINUX_PGTABLE_H */
