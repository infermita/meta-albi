SUMMARY = "A ntpclinet udp"
SECTION = "base"
LICENSE = "CLOSED"

S = "${WORKDIR}"

SRC_URI = "\
	file://ntpclient.c \
	file://ntpclient.in \
	file://resolv.conf \
"

inherit update-rc.d

do_compile() {
	${CC} ${CFLAGS} ${LDFLAGS} ntpclient.c -o ntpclient
}

do_install() {
	install -d ${D}${bindir}
	install -d ${D}/etc
	install -m 0755	${WORKDIR}/ntpclient	${D}${bindir}
	cp -f ${WORKDIR}/resolv.conf ${D}/etc/

	install -d ${D}${sysconfdir}/init.d/
	sed "s:@installpath@:/usr/bin:" ${WORKDIR}/ntpclient.in \
	  > ${D}${sysconfdir}/init.d/ntpclient
	chmod 0755 ${D}${sysconfdir}/init.d/ntpclient
  
  
}
INITSCRIPT_NAME = "ntpclient"
INITSCRIPT_PARAMS = "defaults 75"
	
