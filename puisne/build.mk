#-*-mode:makefile-gmake;indent-tabs-mode:t;tab-width:8;coding:utf-8-*-┐
#───vi: set et ft=make ts=8 tw=8 fenc=utf-8 :vi───────────────────────┘
#
# SYNOPSIS
#
#   Your package build config for executable programs
#
# DESCRIPTION
#
#   We assume each .c file in this directory has a main() function, so
#   that it becomes as easy as possible to write lots of tiny programs
#
# EXAMPLE
#
#   make o//puisne
#   o/puisne/puisne.com
#
# AUTHORS
#
#   Roie McDemigod <roie@mcdemigod.com>

PKGS += PUISNE

# Reads into memory the list of files in this directory.
PUISNE_FILES := $(wildcard puisne/*)

# Defines sets of files without needing further iops.
PUISNE_SRCS = $(filter %.c,$(PUISNE_FILES))
PUISNE_HDRS = $(filter %.h,$(PUISNE_FILES))
PUISNE_COMS = $(PUISNE_SRCS:%.c=o/$(MODE)/%.com)
PUISNE_BINS =					\
	$(PUISNE_COMS)			\
	$(PUISNE_COMS:%=%.dbg)

# Remaps source file names to object names.
# Also asks a wildcard rule to automatically run tool/build/zipobj.c
PUISNE_OBJS =					\
	$(PUISNE_SRCS:%.c=o/$(MODE)/%.o)

# Lists packages whose symbols are or may be directly referenced here.
# Note that linking stubs is always a good idea due to synthetic code.
PUISNE_DIRECTDEPS =    \
	LIBC_TIME          \
	THIRD_PARTY_GETOPT \
	TOOL_ARGS


# Evaluates the set of transitive package dependencies.
PUISNE_DEPS :=				\
	$(call uniq,$(foreach x,$(PUISNE_DIRECTDEPS),$($(x))))

$(PUISNE_A).pkg:				\
		$(PUISNE_OBJS)		\
		$(foreach x,$(PUISNE_DIRECTDEPS),$($(x)_A).pkg)

# Specifies how to build programs as ELF binaries with DWARF debug info.
# @see build/rules.mk for definition of rule that does .com.dbg -> .com
o/$(MODE)/puisne/%.com.dbg:			\
		$(PUISNE_DEPS)		\
		o/$(MODE)/puisne/%.o		\
		o/$(MODE)/puisne/help.txt.zip.o \
		$(CRT)					\
		$(APE_NO_MODIFY_SELF)
	@$(APELINK)

# Invalidates objects in package when makefile is edited.
$(PUISNE_OBJS): puisne/build.mk

# Creates target building everything in package and subpackages.
.PHONY: o/$(MODE)/puisne
o/$(MODE)/puisne:				\
		$(PUISNE_BINS)
