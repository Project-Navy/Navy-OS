/*
 * Copyright (C) 2020  Jordan DALCQ & contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _NAVY_FILESYSTEM_EXT2_H_
#define _NAVY_FILESYSTEM_EXT2_H_



#include <stdint.h>

enum EXT2_STATE
{
    CLEAN = 1,
    HAS_ERRORS
};

enum EXT2_HANDLING
{
    IGNORE = 1,
    REMOUNT_RO,
    PANIC
};

enum EXT2_OSID
{
    LINUX,
    GNU_HURD,
    MASIX,
    FREEBSD,
    OTHER
};

enum EXT2_OPTIONAL_FEATURE
{
    PREALLOCATE_BLOCKS = 1,
    AFS_SERVER = 2,
    EXT3 = 4,
    INODE_EXT_ATTRIBUTES = 8,
    CAN_RESIZE_ITSELF = 16,
    DIR_USE_HASH = 32
};

enum EXT2_REQUIRED_FEATURE
{
    COMPRESSION_USED = 1,
    DIR_CONTAIN_TYPE_FIELD = 2,
    NEED_REPLAY_JOURNAL = 4,
    USE_JOURNAL_DEVICE = 8
};

enum EXT2_RO_FEATURE
{
    SPARSE_SUPERBLOCK = 1,
    USE_64BIT = 2,
    DIR_CONTENT_IN_BINARYTREE = 4
};

enum EXT2_INODE_MODE
{
    FIFO = 0x1000,
    CHAR_DEV = 0x2000,
    DIR = 0x4000,
    BLOCK = 0x6000,
    FILE = 0x8000,
    LINK = 0xA000,
    SOCKET = 0xC000
};

enum EXT2_INODE_FLAG
{
    SECURE_DELETION = 0x1,
    KEEP_COPY_WHEN_DELETE = 0x2,
    FILE_COMPRESSION = 0x4,
    SYNCH_UPDATE = 0x8,
    IMMUTABLE_FILE = 0x10,
    APPEND_ONLY = 0x20,
    FILE_NO_INCLUDED_IN_DUMP = 0x40,
    LAST_ACCESS_TIME_NOT_UPDATED = 0x80,
    HASH_INDEXED_DIRECTORY = 0x10000,
    AFS_DIRECTORY = 0x20000,
    JOURNAL_FILE_DATA = 0x40000
};

enum DIR_ENTRY_TYPE
{
    UNKNOWN,
    REGULAR_ILE,
    DIRECTORY,
    DEVICE,
    BLOCK_DEVICE,
    BUFFER,
    UNIX_SOCKET,
    SYMLINK
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

struct EXT2_BASE_SUPERBLOCK_FIELD
{
    uint32_t s_inodes_count;
    uint32_t s_blocks_count;
    uint32_t s_r_blocks_count;
    uint32_t s_free_blocks_count;
    uint32_t s_free_inodes_count;
    uint32_t s_first_data_block;
    uint32_t s_log_block_size;
    uint32_t s_log_frag_size;
    uint32_t s_blocks_per_group;
    uint32_t s_frags_per_group;
    uint32_t s_inodes_per_group;
    uint16_t s_mtime;
    uint16_t s_wtime;
    uint16_t s_mnt_count;
    uint16_t s_max_mnt_count;
    uint16_t s_magic;
    enum EXT2_STATE s_state:16;
    enum EXT2_HANDLING s_errors:16;
    uint16_t s_minor_rev_level:16;
    uint32_t s_lastcheck;
    uint32_t s_checkinterval;
    enum EXT2_OSID s_creator_os:32;
    uint32_t s_rev_level;
    uint16_t s_def_resuid;
    uint16_t s_def_resgid;

    uint32_t s_first_ino;
    uint16_t s_inode_size;
    uint16_t s_block_group_nr;
    enum EXT2_OPTIONAL_FEATURE s_feature_compat:16; /* Ok if not compatible */
    enum EXT2_REQUIRED_FEATURE s_feature_incompat:16;   /* Not Ok if not compatible */
    enum EXT2_RO_FEATURE s_feature_ro_compat:16;    /* RO if not compatible */
    uint16_t s_uuid;
    uint16_t s_volume_name;
    uint32_t s_last_mounted[16];
    uint32_t s_algo_bitmap;

    uint8_t s_prealloc_blocks;
    uint8_t s_prealloc_dir_blocks;
    uint16_t alignment;

    uint32_t s_journal_uuid[4];
    uint32_t s_journal_inum;
    uint32_t s_journal_dev;
    uint32_t s_last_orphan;
};

struct BLOCK_GROUP_DESCRIPTOR
{
    uint32_t bg_block_bitmap;
    uint32_t bg_inode_bitmap;
    uint32_t bg_inode_table;
    uint16_t bg_free_blocks_count;
    uint16_t bg_free_inodes_count;
    uint16_t bg_used_dirs_count;
    uint32_t bg_reserved[3];
};

struct BLOCK_ARRAY
{
    uint32_t block;
};

union INODE_OS_SPEC
{
    struct
    {
        uint8_t i_frag;
        uint8_t i_fsize;
        uint16_t reserved;
        uint16_t i_uid_high;
        uint16_t gid_high;
        uint32_t reserved2;
    } as_linux;

    struct
    {
        uint8_t i_frag;
        uint8_t i_fsize;
        uint16_t i_mode_high;
        uint16_t i_uid_high;
        uint32_t i_author;
    } as_hurd;

    struct
    {
        uint8_t i_frag;
        uint8_t i_fsize;
        uint16_t reserved[5];
    } as_masix;
};

struct INODE
{
    enum EXT2_INODE_MODE i_mode:16;
    uint16_t i_uid;
    uint32_t i_size;
    uint32_t i_atime;
    uint32_t i_ctime;
    uint32_t i_mtime;
    uint32_t i_dtime;
    uint16_t i_gid;
    uint16_t i_links_count;
    uint32_t i_blocks;
    enum EXT2_INODE_FLAG i_flags:32;
    uint32_t i_osd1;
    struct BLOCK_ARRAY blocks[15];
    uint32_t i_generation;
    uint32_t i_file_acl;
    uint32_t i_dir_acl;
    uint32_t i_faddr;
    union INODE_OS_SPEC i_osd2;
};

#pragma GCC diagnostic push

struct DIR_ENTRY
{
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    enum DIR_ENTRY_TYPE file_type;
    char name[255];
};

#endif
