LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://README.md;md5=3d2c2022d1fee0c9a2e54759f6cd2d49"
SECTION = "multimedia"
DEPENDS += "qtbase libnfc wiringpi"


SRCREV = "a545e8d94b0ea8625a691429df6a12d7732b7208"
SRC_URI = "git://github.com/infermita/alubadge.git"

SRC_URI += "file://alubadge.in \
	    file://modules \
"
S = "${WORKDIR}/git"

inherit qmake5 systemd update-rc.d

do_install () {
  install -d ${D}/usr/bin/
  install -d ${D}/etc


  cp -f ${WORKDIR}/build/alubadge ${D}/usr/bin/
  cp -f ${WORKDIR}/modules ${D}/etc/

  if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
	install -d ${D}${sysconfdir}/init.d/
	sed "s:@installpath@:/usr/bin:" ${WORKDIR}/alubadge.in \
	  > ${D}${sysconfdir}/init.d/alubadge
	chmod 0755 ${D}${sysconfdir}/init.d/alubadge
  fi
  
  
}
INITSCRIPT_NAME = "alubadge"
INITSCRIPT_PARAMS = "defaults 88"
