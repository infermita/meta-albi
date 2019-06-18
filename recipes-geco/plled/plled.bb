SUMMARY = "C++ library for client-side URL transfers"
HOMEPAGE = "http://code.google.com/p/curlpp/"
SECTION = "multimedia"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

DEPENDS = "curlpp wiringpi sqlite3"

S = "${WORKDIR}"

inherit autotools

SRC_URI += "file://main.cpp \
           file://Rs485.cpp \
	   file://Rs485.h \
	   file://Makefile.am \
	   file://configure.ac \
"

do_install () {

	install -d ${D}/usr/bin/
	cp -f ${WORKDIR}/build/plled ${D}/usr/bin/    

}

