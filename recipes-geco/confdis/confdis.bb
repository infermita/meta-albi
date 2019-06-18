LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://LICENSE;md5=d41d8cd98f00b204e9800998ecf8427e"
SECTION = "multimedia"
DEPENDS += "qtbase libnfc wiringpi"
SRCREV = "182"
SRC_URI = "svn://svn.gecoitalia.biz:18080/svn/confdis/software;protocol=http;module=client"

SRC_URI += "file://confdis.in \
           file://confdis.service \
	   file://DejaVuSans.ttf \
"
S = "${WORKDIR}/client/"

inherit qmake5 systemd update-rc.d

SYSTEMD_SERVICE_${PN} = "confdis.service"
SYSTEMD_AUTO_ENABLE_${PN} = "enable"

FILES_${PN} += "${libdir}/*"

do_install () {
  install -d ${D}${sysconfdir}/confdis
  install -d ${D}${base_libdir}/systemd/system
  install -d ${D}/usr/bin/
  install -d ${D}/usr/lib/fonts

  rm -f ${S}/db/confdis.db
  sh ${S}/db/restoredb.sh

  cp -f ${S}/db/confdis.db ${D}${sysconfdir}/confdis/
  cp -f ${S}/images/defaultimg.jpg ${D}${sysconfdir}/confdis/
  cp -f ${S}/confdis.ini ${D}${sysconfdir}/confdis/
  cp -f ${WORKDIR}/build/confdis ${D}/usr/bin/
  cp -f ${WORKDIR}/DejaVuSans.ttf ${D}/usr/lib/fonts/

  install -m 0644 ${WORKDIR}/confdis.service ${D}${base_libdir}/systemd/system/

  if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
	install -d ${D}${sysconfdir}/init.d/
	sed "s:@installpath@:/usr/bin:" ${WORKDIR}/confdis.in \
	  > ${D}${sysconfdir}/init.d/confdis
	chmod 0755 ${D}${sysconfdir}/init.d/confdis
  fi
  #echo "conf:12345:respawn:/usr/bin/confdis -platform linuxfb" >> ${D}${sysconfdir}/inittab
  
}
INITSCRIPT_NAME = "confdis"
INITSCRIPT_PARAMS = "defaults 86"
