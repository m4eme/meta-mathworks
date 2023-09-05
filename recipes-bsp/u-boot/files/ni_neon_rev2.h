/* SPDX-License-Identifier: GPL-2.0
 *
 * (C) Copyright 2016,2017 National Instruments Corp
 *
 * Configuration settings for the NI Project Neon Rev2 SDR board
 * See zynq-common.h for Zynq common configs
 *
 */
#ifndef __CONFIG_NI_NEON_REV2_H
#define __CONFIG_NI_NEON_REV2_H

#define CONFIG_ENV_IS_NOWHERE
#include <configs/zynq-common.h>
#include <configs/mw_xilinx_common.h>
#undef CONFIG_ENV_IS_NOWHERE

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"fit_image=boot/fitImage\0"		\
	"ec_compat=2\0"		\
	"mboard=neon-rev1\0"	\
	"ec_disable_swsync=0\0"		\
	"ec_image=lib/firmware/ni/ec-neon-rev2.RW.bin\0"	\
	"expand_ec_image=setenv ec_image lib/firmware/ni/ec-neon-rev${ec_compat}.RW.bin\0" \
	"bootargs=root=${mender_kernel_root} rw rootwait uio_pdrv_genirq.of_id=usrp-uio\0" \
	"load_addr=0x2000000\0"		\
	"fit_size=0x800000\0"           \
	"fdt_high=0x20000000\0"         \
	"initrd_high=0x20000000\0"      \
	"netargs=printenv serverip && printenv tftproot && printenv nfsroot && setenv bootargs root=/dev/nfs " \
		"nfsroot=${serverip}:${nfsroot},v3,tcp ip=dhcp uio_pdrv_genirq.of_id=usrp-uio\0" \
	"netboot=run netargs && run swsync_net && " \
		"echo Loading FIT to RAM via DHCP/TFTP && " \
		"dhcp ${load_addr} ${tftproot}/${fit_image} && "\
		"bootm ${load_addr}#conf@zynq-ni-${mboard}.dtb\0" \
	"sdboot=run mender_setup; run swsync_sd; " \
		"echo Copying FIT from SD to RAM... && " \
		"ext4load ${mender_uboot_root} ${load_addr} ${fit_image} && " \
		"bootm ${load_addr}#conf@zynq-ni-${mboard}.dtb\0" \
	"swsync_sd=if test ${ec_disable_swsync} = 0; then "\
			"echo Copying MCU FW from SD to RAM... && " \
			"run expand_ec_image; " \
			"ext4load ${mender_uboot_root} ${load_addr} ${ec_image} && " \
			"crosec swsync ${load_addr} ${filesize}; fi;\0" \
	"swsync_net=if test ${ec_disable_swsync} = 0; then " \
			"echo Copying MCU FW to RAM via DHCP/TFTP... && " \
			"run expand_ec_image; " \
			"dhcp ${load_addr} ${tftproot}/${ec_image} && " \
			"crosec swsync ${load_addr} ${filesize}; fi;\0" \
	"jtagboot=echo TFTPing FIT to RAM... && " \
		"tftpboot ${load_addr} ${fit_image} && " \
		"bootm ${load_addr}#conf@zynq-ni-${mboard}.dtb\0" \
	"usbboot=if usb start; then " \
			"echo Copying FIT from USB to RAM... && " \
			"load usb 0 ${load_addr} ${fit_image} && " \
			"bootm ${load_addr}#conf@zynq-ni-${mboard}.dtb; fi\0" \
  "kernel_addr=0x10000000\0" \
	"initrd_addr=0x2000000\0" \
	"initrd_high=0x10000000\0" \
	"initrd_size=0x2000000\0" \
	"initrd_image=uramdisk.image.gz\0"	\
	"fdt_addr=23fff000\0" \
        "fdtov_addr=240bf000\0" \
	"fdt_high=0x10000000\0" \
	"fdt_image=devicetree.dtb\0"	\
	"bitstream_addr=0x1000000\0"	\
	"bitstream_image=system.bit\0"	\
	"nc=setenv stdout nc;setenv stdin nc;\0" \
	"ethaddr=00:0a:35:00:22:01\0" \
	"bootenv=uEnv.txt\0" \
	"importbootenv=echo \"Importing environment from SD ...\"; " \
		"env import -t ${loadbootenv_addr} $filesize\0" \
	"loadbootenv=load mmc $sdbootdev:$partid ${loadbootenv_addr} ${bootenv}\0" \
	"sd_uEnvtxt_existence_test=test -e mmc $sdbootdev:$partid /uEnv.txt\0" \
	"uenvboot=" \
		"if run sd_uEnvtxt_existence_test; then " \
			"run loadbootenv; " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
		"fi; " \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...; " \
			"run uenvcmd; " \
		"fi\0" \
    "mmc_loadbit=echo Loading bitstream from SD/MMC/eMMC to RAM.. && " \
		"mmcinfo && " \
		"load mmc $sdbootdev:$partid ${bitstream_addr} ${bitstream_image} && " \
		"fpga loadb $sdbootdev:$partid ${bitstream_addr} ${filesize}\0" \
	"sd_bitstream_existence_test=test -e mmc $sdbootdev:$partid /${bitstream_image}\0" \
	"sd_boot_loadbit=" \
		"if run sd_bitstream_existence_test; then " \
			"run mmc_loadbit;" \
		"fi; \0" \
        "sd_dto_existence_test=test -e mmc $sdbootdev:$partid /${fdt_overlay}\0" \
        "sd_apply_overlay=" \
                "if run sd_dto_existence_test; then " \
                    "fdt addr $fdt_addr; && " \
                    "load mmc $sdbootdev:$partid $fdtov_addr $fdt_overlay && " \
                    "fdt resize ${filesize}; && "\
                    "fdt apply $fdtov_addr; && "\
                    "echo Applied Device-Tree Overlay: ${fdt_overlay}... && " \
                "fi; \0" \
    ENV_CMD_INIT_ENV_ONCE \
	"sdboot=if mmc dev $sdbootdev && mmcinfo; then " \
			"run uenv_init; " \
			"run uenvboot; " \
			"run sd_boot_loadbit; " \
			"echo Copying Linux from SD to RAM... && " \
			"load mmc $sdbootdev:$partid $kernel_addr Image && " \
			"load mmc $sdbootdev:$partid $fdt_addr $fdt_image && " \
                        "if test -n ${fdt_overlay}; then " \
                            "run sd_apply_overlay; " \
                        "fi; " \    
			"if load mmc 0 ${initrd_addr} ${initrd_image}; then " \
				"booti ${kernel_addr} ${initrd_addr} ${fdt_addr}; " \
			"else " \
				"booti ${kernel_addr} - ${fdt_addr}; " \
			"fi &&" \
		"fi\0" \
	"autoload=no\0" \
	"sdbootdev=0\0" \
	"clobstart=0x10000000\0" \
	"netstart=0x10000000\0" \
	"dtbnetstart=0x23fff000\0" \
	"loadaddr=0x10000000\0" \
	"boot_img=BOOT.BIN\0" \
	"load_boot=tftpboot ${clobstart} ${boot_img}\0" \
	"update_boot=setenv img boot; setenv psize ${bootsize}; setenv installcmd \"install_boot\"; run load_boot ${installcmd}; setenv img; setenv psize; setenv installcmd\0" \
	"install_boot=mmcinfo && fatwrite mmc ${sdbootdev} ${clobstart} ${boot_img} ${filesize}\0" \
	"bootenvsize=0x40000\0" \
	"bootenvstart=0x1e00000\0" \
	"eraseenv=sf probe 0 && sf erase ${bootenvstart} ${bootenvsize}\0" \
	"jffs2_img=rootfs.jffs2\0" \
	"load_jffs2=tftpboot ${clobstart} ${jffs2_img}\0" \
	"update_jffs2=setenv img jffs2; setenv psize ${jffs2size}; setenv installcmd \"install_jffs2\"; run load_jffs2 test_img; setenv img; setenv psize; setenv installcmd\0" \
	"sd_update_jffs2=echo Updating jffs2 from SD; mmcinfo && fatload mmc ${sdbootdev}:1 ${clobstart} ${jffs2_img} && run install_jffs2\0" \
	"install_jffs2=sf probe 0 && sf erase ${jffs2start} ${jffs2size} && " \
		"sf write ${clobstart} ${jffs2start} ${filesize}\0" \
	"kernel_img=Image\0" \
	"load_kernel=tftpboot ${clobstart} ${kernel_img}\0" \
	"update_kernel=setenv img kernel; setenv psize ${kernelsize}; setenv installcmd \"install_kernel\"; run load_kernel ${installcmd}; setenv img; setenv psize; setenv installcmd\0" \
	"install_kernel=mmcinfo && fatwrite mmc ${sdbootdev} ${clobstart} ${kernel_img} ${filesize}\0" \
	"cp_kernel2ram=mmcinfo && fatload mmc ${sdbootdev} ${netstart} ${kernel_img}\0" \
	"dtb_img=system.dtb\0" \
	"load_dtb=tftpboot ${clobstart} ${dtb_img}\0" \
	"update_dtb=setenv img dtb; setenv psize ${dtbsize}; setenv installcmd \"install_dtb\"; run load_dtb ${installcmd}; setenv img; setenv psize; setenv installcmd\0" \
	"install_dtb=mmcinfo && fatwrite mmc ${sdbootdev} ${clobstart} ${dtb_img} ${filesize}\0" \
	"cp_dtb2ram=mmcinfo && fatload mmc ${sdbootdev}:1 ${dtbnetstart} ${dtb_img}\0" \
	"loadbootenv_addr=0x00100000\0" \
	"fault=echo ${img} image size is greater than allocated place - partition ${img} is NOT UPDATED\0" \
	"test_crc=if imi ${clobstart}; then run test_img; else echo ${img} Bad CRC - ${img} is NOT UPDATED; fi\0" \
	"test_img=setenv var \"if test ${filesize} -gt ${psize}\\; then run fault\\; else run ${installcmd}\\; fi\"; run var; setenv var\0" \
	"netboot=tftpboot ${netstart} ${kernel_img} && bootm\0" \
	"default_bootcmd=run uenvboot; run cp_kernel2ram && run cp_dtb2ram && booti ${netstart} - ${dtbnetstart}\0" \
  DFU_ALT_INFO


#endif /* __CONFIG_NI_NEON_REV2_H */
