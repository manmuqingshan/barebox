// SPDX-License-Identifier: GPL-2.0-only

#include <stdio.h>
#include <common.h>
#include <config.h>
#include <mach/mpc5xxx.h>
#include <command.h>
#include <complete.h>
#include <asm/io.h>

static int do_reginfo(int argc, char *argv[])
{
	puts ("\nMPC5200 registers\n");
	printf ("MBAR=%08x\n", CFG_MBAR);
	puts ("Memory map registers\n");
	printf ("\tCS0: start %08X\tstop %08X\tconfig %08X\ten %d\n",
		in_be32((void*)MPC5XXX_CS0_START),
		in_be32((void*)MPC5XXX_CS0_STOP),
		in_be32((void*)MPC5XXX_CS0_CFG),
		in_be32((void*)MPC5XXX_ADDECR) & 0x00010000 ? 1 : 0);
	printf ("\tCS1: start %08X\tstop %08X\tconfig %08X\ten %d\n",
		in_be32((void*)MPC5XXX_CS1_START),
		in_be32((void*)MPC5XXX_CS1_STOP),
		in_be32((void*)MPC5XXX_CS1_CFG),
		in_be32((void*)MPC5XXX_ADDECR) & 0x00020000 ? 1 : 0);
	printf ("\tCS2: start %08X\tstop %08X\tconfig %08X\ten %d\n",
		in_be32((void*)MPC5XXX_CS2_START),
		in_be32((void*)MPC5XXX_CS2_STOP),
		in_be32((void*)MPC5XXX_CS2_CFG),
		in_be32((void*)MPC5XXX_ADDECR) & 0x00040000 ? 1 : 0);
	printf ("\tCS3: start %08X\tstop %08X\tconfig %08X\ten %d\n",
		in_be32((void*)MPC5XXX_CS3_START),
		in_be32((void*)MPC5XXX_CS3_STOP),
		in_be32((void*)MPC5XXX_CS3_CFG),
		in_be32((void*)MPC5XXX_ADDECR) & 0x00080000 ? 1 : 0);
	printf ("\tCS4: start %08X\tstop %08X\tconfig %08X\ten %d\n",
		in_be32((void*)MPC5XXX_CS4_START),
		in_be32((void*)MPC5XXX_CS4_STOP),
		in_be32((void*)MPC5XXX_CS4_CFG),
		in_be32((void*)MPC5XXX_ADDECR) & 0x00100000 ? 1 : 0);
	printf ("\tCS5: start %08X\tstop %08X\tconfig %08X\ten %d\n",
		in_be32((void*)MPC5XXX_CS5_START),
		in_be32((void*)MPC5XXX_CS5_STOP),
		in_be32((void*)MPC5XXX_CS5_CFG),
		in_be32((void*)MPC5XXX_ADDECR) & 0x00200000 ? 1 : 0);
	printf ("\tCS6: start %08X\tstop %08X\tconfig %08X\ten %d\n",
		in_be32((void*)MPC5XXX_CS6_START),
		in_be32((void*)MPC5XXX_CS6_STOP),
		in_be32((void*)MPC5XXX_CS6_CFG),
		in_be32((void*)MPC5XXX_ADDECR) & 0x04000000 ? 1 : 0);
	printf ("\tCS7: start %08X\tstop %08X\tconfig %08X\ten %d\n",
		in_be32((void*)MPC5XXX_CS7_START),
		in_be32((void*)MPC5XXX_CS7_STOP),
		in_be32((void*)MPC5XXX_CS7_CFG),
		in_be32((void*)MPC5XXX_ADDECR) & 0x08000000 ? 1 : 0);
	printf ("\tBOOTCS: start %08X\tstop %08X\tconfig %08X\ten %d\n",
		in_be32((void*)MPC5XXX_BOOTCS_START),
		in_be32((void*)MPC5XXX_BOOTCS_STOP),
		in_be32((void*)MPC5XXX_BOOTCS_CFG),
		in_be32((void*)MPC5XXX_ADDECR) & 0x02000000 ? 1 : 0);
	printf ("\tSDRAMCS0: %08X\n",
		in_be32((void*)MPC5XXX_SDRAM_CS0CFG));
	printf ("\tSDRAMCS1: %08X\n",
		in_be32((void*)MPC5XXX_SDRAM_CS1CFG));

	return 0;
}

BAREBOX_CMD_START(reginfo)
       .cmd            = do_reginfo,
       BAREBOX_CMD_DESC("print MPC5200 register information")
       BAREBOX_CMD_GROUP(CMD_GRP_INFO)
       BAREBOX_CMD_COMPLETE(empty_complete)
BAREBOX_CMD_END
