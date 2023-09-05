FILESEXTRAPATHS_prepend := "${THISDIR}/files/zynqmp/fs-overlay/etc/:"
#SRC_URI += " file://fw_env.config"
SRC_URI += " file://fstab"
# This file cannot belong to base_files and mw-fs-overlay at the same time, so we need to add it here. Otherwise the default base_files version willcomplain...
FILES_${PN} += "${sysconfdir}/fstab"

do_install_append() {
    install -d ${D}${sysconfdir}
#    install -m 0644 ${WORKDIR}/fw_env.config  ${D}${sysconfdir}/fw_env.config
    install -m 0644 ${WORKDIR}/fstab  ${D}${sysconfdir}/fstab
}

PACKAGE_ARCH = "${MACHINE_ARCH}"
