# look for files in the layer first
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"


INITSCRIPT_PARAMS = "start 74 2 3 4 5 . stop 80 0 6 1 ."
