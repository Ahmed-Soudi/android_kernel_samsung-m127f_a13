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

#endif /* _LINUX_PGTABLE_H */
