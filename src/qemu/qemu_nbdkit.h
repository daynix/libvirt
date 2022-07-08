/*
 * qemu_nbdkit.h: helpers for using nbdkit
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "internal.h"
#include "storage_source_conf.h"
#include "virenum.h"
#include "virfilecache.h"

typedef struct _qemuNbdkitCaps qemuNbdkitCaps;
typedef struct _qemuNbdkitProcess qemuNbdkitProcess;

typedef enum {
    /* 0 */
    QEMU_NBDKIT_CAPS_PLUGIN_CURL,
    QEMU_NBDKIT_CAPS_PLUGIN_SSH,
    QEMU_NBDKIT_CAPS_FILTER_READAHEAD,

    QEMU_NBDKIT_CAPS_LAST,
} qemuNbdkitCapsFlags;

VIR_ENUM_DECL(qemuNbdkitCaps);

typedef struct _virQEMUDriver virQEMUDriver;

qemuNbdkitCaps *
qemuNbdkitCapsNew(const char *path);

virFileCache *
qemuNbdkitCapsCacheNew(const char *cachedir);

bool
qemuNbdkitInitStorageSource(qemuNbdkitCaps *nbdkitCaps,
                            virStorageSource *source,
                            char *statedir,
                            const char *alias,
                            uid_t user,
                            gid_t group);

bool
qemuNbdkitCapsGet(qemuNbdkitCaps *nbdkitCaps,
                  qemuNbdkitCapsFlags flag);

void
qemuNbdkitCapsSet(qemuNbdkitCaps *nbdkitCaps,
                  qemuNbdkitCapsFlags flag);

#define QEMU_TYPE_NBDKIT_CAPS qemu_nbdkit_caps_get_type()
G_DECLARE_FINAL_TYPE(qemuNbdkitCaps, qemu_nbdkit_caps, QEMU, NBDKIT_CAPS, GObject);

struct _qemuNbdkitProcess {
    qemuNbdkitCaps *caps;
    virStorageSource *source;

    char *pidfile;
    char *socketfile;
    uid_t user;
    gid_t group;
    pid_t pid;
};

int
qemuNbdkitProcessStart(qemuNbdkitProcess *proc,
                       virDomainObj *vm,
                       virQEMUDriver *driver);

int
qemuNbdkitProcessStop(qemuNbdkitProcess *proc);

void
qemuNbdkitProcessFree(qemuNbdkitProcess *proc);

G_DEFINE_AUTOPTR_CLEANUP_FUNC(qemuNbdkitProcess, qemuNbdkitProcessFree);
