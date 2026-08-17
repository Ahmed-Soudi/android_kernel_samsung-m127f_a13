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
 * KernelSU-Next v3.3.0 includes <linux/pgtable.h>.
 * Forwarding to the existing Samsung 4.19 MM interface keeps newer
 * KernelSU-Next code compatible without importing a newer kernel's
 * page-table implementation.
 */

#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/version.h>

/*
 * KernelSU-Next compatibility for Samsung Linux 4.19
 *
 * Newer KernelSU-Next code expects several no-fault helpers introduced
 * after this vendor kernel:
 *
 *   strncpy_from_user_nofault()
 *   copy_to_kernel_nofault()
 *
 * Samsung 4.19 already has the primitives needed to implement compatible
 * behaviour.
 */

/*
 * strncpy_from_user_nofault()
 *
 * Perform a user-string copy while page faults are disabled.
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

#endif /* KSU_HAVE_STRNCPY_FROM_USER_NOFAULT */
#endif /* < 5.8 */


/*
 * copy_to_kernel_nofault()
 *
 * Modern kernels provide this helper for fault-safe writes between kernel
 * addresses.
 *
 * Samsung 4.19 already provides probe_kernel_write(), whose semantics match
 * what KernelSU requires here:
 *
 *   0       successful copy
 *   -EFAULT destination/source fault
 *
 * In KernelSU-Next v3.3.0 this is used by patch_memory.c to write patch data
 * into a temporary writable fixmap mapping.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 8, 0)
#ifndef KSU_HAVE_COPY_TO_KERNEL_NOFAULT

static inline long
ksu_copy_to_kernel_nofault(void *dst,
                           const void *src,
                           size_t size)
{
    if (unlikely(!size))
        return 0;

    return probe_kernel_write(dst, src, size);
}

#define copy_to_kernel_nofault ksu_copy_to_kernel_nofault

#endif /* KSU_HAVE_COPY_TO_KERNEL_NOFAULT */
#endif /* < 5.8 */

#endif /* _LINUX_PGTABLE_H */
