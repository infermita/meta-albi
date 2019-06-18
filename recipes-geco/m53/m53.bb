SUMMARY = "C++ library for client-side URL transfers"
HOMEPAGE = "http://code.google.com/p/curlpp/"
SECTION = "multimedia"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

DEPENDS = "gd wiringpi"

S = "${WORKDIR}"

inherit autotools update-rc.d

SRC_URI += "file://main.c \
           file://bmgcom.c \
	   file://drawImage.c \
	   file://modem.c \
	   file://Makefile.am \
	   file://configure.ac \
	   file://NEWS \
	   file://AUTHORS \
	   file://ChangeLog \
	   file://README \
	   file://m53.in \
"

do_install () {

	install -d ${D}/usr/bin/
	cp -f ${WORKDIR}/build/m53 ${D}/usr/bin/    
	
	install -d ${D}${sysconfdir}/init.d/
	sed "s:@installpath@:/usr/bin:" ${WORKDIR}/m53.in \
	  > ${D}${sysconfdir}/init.d/m53
	chmod 0755 ${D}${sysconfdir}/init.d/m53

}
INITSCRIPT_NAME = "m53"
INITSCRIPT_PARAMS = "defaults 88"

