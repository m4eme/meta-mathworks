SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-yocto-4.18:"
SRC_URI_append += " \
	file://mw.cfg \
	file://mw.scc \
"
