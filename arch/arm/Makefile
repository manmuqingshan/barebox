# SPDX-License-Identifier: GPL-2.0-only

KBUILD_DEFCONFIG := multi_v8_defconfig

generated_configs += am335x_mlo_sdmmc_defconfig am335x_mlo_rawflash_defconfig
am335x_mlo_sdmmc_defconfig:
	$(call merge_into_defconfig,am335x_mlo_defconfig,nomtd)
am335x_mlo_rawflash_defconfig:
	$(call merge_into_defconfig,am335x_mlo_defconfig,nomci)

KBUILD_CPPFLAGS	+= -D__ARM__ -fno-strict-aliasing
# Explicitly specifiy 32-bit ARM ISA since toolchain default can be -mthumb:
ifeq ($(CONFIG_CPU_64),y)
KBUILD_CPPFLAGS	+=$(call cc-option,-maarch64,)
endif
ifeq ($(CONFIG_CPU_32),y)
KBUILD_CPPFLAGS	+=$(call cc-option,-marm,)
KBUILD_CPPFLAGS	+= -msoft-float
endif

ifeq ($(CONFIG_CPU_BIG_ENDIAN),y)
KBUILD_CPPFLAGS	+= -mbig-endian
LD		+= -EB
else
KBUILD_CPPFLAGS	+= -mlittle-endian
LD		+= -EL
endif

ifeq ($(CONFIG_ARM_MODULE_PLTS),y)
LDFLAGS_MODULE   += -T $(srctree)/arch/arm/lib32/module.lds
endif

# Unaligned access is not supported when MMU is disabled, so given how
# at least some of the code would be executed with MMU off, lets be
# conservative and instruct the compiler not to generate any unaligned
# accesses
ifeq ($(CONFIG_CPU_32),y)
KBUILD_CFLAGS += -mno-unaligned-access
endif
ifeq ($(CONFIG_CPU_64),y)
KBUILD_CFLAGS += -mstrict-align
# Prevent use of floating point and Advanced SIMD registers.
KBUILD_CFLAGS += -mgeneral-regs-only
endif

# This selects which instruction set is used.
# Note that GCC does not numerically define an architecture version
# macro, but instead defines a whole series of macros which makes
# testing for a specific architecture or later rather impossible.
arch-y				:= -include asm/arch-check.h
arch-$(CONFIG_CPU_64v8)		:= -D__LINUX_ARM_ARCH__=8 $(call cc-option,-march=armv8-a)
arch-$(CONFIG_CPU_32v7)		:=-D__LINUX_ARM_ARCH__=7 $(call cc-option,-march=armv7-a,-march=armv5t -Wa$(comma)-march=armv7-a)
arch-$(CONFIG_CPU_32v6)		:=-D__LINUX_ARM_ARCH__=6 $(call cc-option,-march=armv6,-march=armv5t -Wa$(comma)-march=armv6)
arch-$(CONFIG_CPU_32v5)		:=-D__LINUX_ARM_ARCH__=5 $(call cc-option,-march=armv5te,-march=armv4t)
arch-$(CONFIG_CPU_32v4T)	:=-D__LINUX_ARM_ARCH__=4 -march=armv4t

# This selects how we optimise for the processor.
tune-$(CONFIG_CPU_ARM920T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_ARM926T)	:=-mtune=arm9tdmi
tune-$(CONFIG_CPU_XSCALE)	:=$(call cc-option,-mtune=xscale,-mtune=strongarm110) -Wa,-mcpu=xscale

CFLAGS_ABI-$(CONFIG_CPU_64)	:=-mabi=lp64
CFLAGS_ABI-$(CONFIG_CPU_32)	:=$(call cc-option,-mapcs-32,-mabi=apcs-gnu) $(call cc-option,-mno-thumb-interwork,)
CFLAGS_ABI-$(CONFIG_AEABI)	:=-mabi=aapcs-linux

CFLAGS_ABI-$(CONFIG_ARM_UNWIND)	+=-funwind-tables

ifeq ($(CONFIG_THUMB2_BAREBOX),y)
AFLAGS_AUTOIT	:=$(call as-option,-Wa$(comma)-mimplicit-it=always,-Wa$(comma)-mauto-it)
AFLAGS_NOWARN	:=$(call as-option,-Wa$(comma)-mno-warn-deprecated,-Wa$(comma)-W)
CFLAGS_THUMB2	:=-mthumb $(AFLAGS_AUTOIT) $(AFLAGS_NOWARN)
AFLAGS_THUMB2	:=$(CFLAGS_THUMB2) -Wa$(comma)-mthumb
endif

KBUILD_CPPFLAGS += $(CFLAGS_ABI-y) $(arch-y) $(tune-y)

ifeq ($(CONFIG_CPU_64), y)
KBUILD_AFLAGS   += -include asm/unified.h
export S64_32 = 64
export S64 = 64
endif
ifeq ($(CONFIG_CPU_32), y)
KBUILD_CPPFLAGS += $(CFLAGS_THUMB2)
KBUILD_AFLAGS   += -include asm/unified.h -msoft-float $(AFLAGS_THUMB2)
export S64_32 = 32
export S32 = 32
endif

# Machine directory name.  This list is sorted alphanumerically
# by CONFIG_* macro name.
machine-$(CONFIG_ARCH_AT91)		+= at91
machine-$(CONFIG_ARCH_BCM283X)		+= bcm283x
machine-$(CONFIG_ARCH_CLPS711X)		+= clps711x
machine-$(CONFIG_ARCH_IMX)		+= imx
machine-$(CONFIG_ARCH_K3)		+= k3
machine-$(CONFIG_ARCH_LAYERSCAPE)	+= layerscape
machine-$(CONFIG_ARCH_MXS)		+= mxs
machine-$(CONFIG_ARCH_MVEBU)		+= mvebu
machine-$(CONFIG_ARCH_OMAP)		+= omap
machine-$(CONFIG_ARCH_PXA)		+= pxa
machine-$(CONFIG_ARCH_ROCKCHIP)		+= rockchip
machine-$(CONFIG_ARCH_SOCFPGA)		+= socfpga
machine-$(CONFIG_ARCH_STM32MP)		+= stm32mp
machine-$(CONFIG_ARCH_SUNXI)		+= sunxi
machine-$(CONFIG_ARCH_VERSATILE)	+= versatile
machine-$(CONFIG_ARCH_VEXPRESS)		+= vexpress
machine-$(CONFIG_ARCH_TEGRA)		+= tegra
machine-$(CONFIG_ARCH_ZYNQ)		+= zynq
machine-$(CONFIG_ARCH_ZYNQMP)		+= zynqmp

# Add cleanup flags
KBUILD_CPPFLAGS += -fdata-sections -ffunction-sections
LDFLAGS_barebox += --gc-sections -pie
LDFLAGS_pbl += --gc-sections -pie

# early code often runs at addresses we are not linked at
KBUILD_CFLAGS_KERNEL += -fPIE

KBUILD_BINARY := barebox.bin

archclean:
	$(MAKE) $(clean)=$(pbl)

dts := arch/arm/dts

%.dtb: scripts
	$(Q)$(MAKE) $(build)=$(dts) $(dts)/$@

KBUILD_IMAGE ?= $(KBUILD_BINARY)

ifneq ($(machine-y),)
MACH := $(patsubst %,arch/arm/mach-%/,$(machine-y))
else
MACH :=
endif

common-y += arch/arm/boards/ $(MACH)
common-y += arch/arm/cpu/
common-y += arch/arm/crypto/

# For cleaning
common- += $(patsubst %,arch/arm/mach-%/,$(machine-))

ifeq ($(CONFIG_CPU_64), y)
common-y += arch/arm/lib64/
else
common-y += arch/arm/lib32/
endif

common-$(CONFIG_OFTREE) += arch/arm/dts/

ifeq ($(CONFIG_CPU_64), y)
lds-y	:= arch/arm/lib64/barebox.lds
else
lds-y	:= arch/arm/lib32/barebox.lds
endif

common- += $(patsubst %,arch/arm/boards/%/,$(board-))

CLEAN_FILES += barebox-flash-image
CLEAN_FILES += arch/arm/lib64/barebox.lds
CLEAN_FILES += arch/arm/lib32/barebox.lds
