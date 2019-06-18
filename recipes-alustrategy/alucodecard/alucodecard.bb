LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://README.md;md5=3d2c2022d1fee0c9a2e54759f6cd2d49"
SECTION = "multimedia"
DEPENDS += "qtbase libnfc wiringpi"


SRCREV = "926452ec4aa01d30e54773b513ef298969852e0d"
SRC_URI = "git://github.com/infermita/alucodecard.git"

SRC_URI += "file://alucodecard.in \
	    file://modules \
"
S = "${WORKDIR}/git"

inherit qmake5 systemd update-rc.d

do_install () {
  install -d ${D}/usr/bin/
  install -d ${D}/etc


  cp -f ${WORKDIR}/build/alucodecard ${D}/usr/bin/
  cp -f ${WORKDIR}/modules ${D}/etc/

  if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
	install -d ${D}${sysconfdir}/init.d/
	sed "s:@installpath@:/usr/bin:" ${WORKDIR}/alucodecard.in \
	  > ${D}${sysconfdir}/init.d/alucodecard
	chmod 0755 ${D}${sysconfdir}/init.d/alucodecard
  fi
  
  
}
INITSCRIPT_NAME = "alucodecard"
INITSCRIPT_PARAMS = "defaults 88"
