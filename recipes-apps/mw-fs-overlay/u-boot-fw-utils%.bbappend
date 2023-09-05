FILESEXTRAPATHS_prepend := "${THISDIR}/files/common/fs-overlay/etc/:"
#S = "${WORKDIR}/mw-fs-overlay"
#S = "${WORKDIR}"
SRC_URI_append += " file://fw_env.config\
                  "

B = "${WORKDIR}/build"

do_install_append() {
    install -d ${D}${sysconfdir}
    install -m 0644 ${WORKDIR}/fw_env.config  ${D}${sysconfdir}/fw_env.config
}
PACKAGE_ARCH = "${MACHINE_ARCH}"
