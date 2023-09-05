EXTRAPATHS_prepend += "${THISDIR}/files:"

SRC_URI += "file://zynqmp/fs-overlay/etc/ \ 
  	        file://zynqmp/fs-overlay/etc/profile.d/ \
"

do_install_append() {
install -d ${D}/${sysconfdir}/profile.d/
	cp -r ${WORKDIR}/zynqmp/fs-overlay/etc/profile.d/* ${D}${sysconfdir}/profile.d/

install -d ${D}/${sysconfdir}/bootvars.d/
	cp -r ${WORKDIR}/zynqmp/fs-overlay/etc/bootvars.d/* ${D}${sysconfdir}/bootvars.d/
}
