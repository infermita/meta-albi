LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://LICENSE;md5=d41d8cd98f00b204e9800998ecf8427e"
SECTION = "multimedia"
DEPENDS += "qtbase libnfc wiringpi"


SRCREV = "66b640bd5e109b3e4438ba7d0778e70bdeb4cb22"

SRC_URI = "git://github.com/infermita/alucountpieces.git"

SRC_URI += "file://alucount.in \
	    file://modules \
	    file://88-net.rules \
"
S = "${WORKDIR}/git"

inherit qmake5 systemd update-rc.d

do_install () {
  install -d ${D}/usr/bin/
  install -d ${D}/etc/
  install -d ${D}/etc/alucount/
  install -d ${D}/etc/udev/rules.d/


  cp -f ${WORKDIR}/build/AluCountPieces ${D}/usr/bin/
  cp -f ${WORKDIR}/modules ${D}/etc/
  cp -f ${WORKDIR}/88-net.rules ${D}/etc/udev/rules.d/
  cp -f ${S}/conf.ini ${D}/etc/alucount/

  if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
	install -d ${D}${sysconfdir}/init.d/
	sed "s:@installpath@:/usr/bin:" ${WORKDIR}/alucount.in \
	  > ${D}${sysconfdir}/init.d/alucount
	chmod 0755 ${D}${sysconfdir}/init.d/alucount
  fi
  
  
}
INITSCRIPT_NAME = "alucount"
INITSCRIPT_PARAMS = "defaults 88"
