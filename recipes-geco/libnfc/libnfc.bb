SUMMARY = "Public platform independent Near Field Communication (NFC) library"
DESCRIPTION = "libnfc is a library which allows userspace application access to NFC devices."
HOMEPAGE = "http://www.libnfc.org/"
BUGTRACKER = "http://code.google.com/p/libnfc/issues/list"

DEPENDS += "libusb"

LICENSE     = "LGPLv3"
LIC_FILES_CHKSUM = "file://COPYING;md5=b52f2d57d10c4f7ee67a7eb9615d5d24"

S = "${WORKDIR}/git"

SRC_URI = "git://github.com/nfc-tools/libnfc.git"
SRCREV = "8e5ec4acf3da44c4804283f1cceb3489ed861c24"

SRC_URI += " \
        file://pn532_uart_on_rpi.conf \
"

inherit autotools pkgconfig

do_install_append () {

	#autotools_do_install
	install -d ${D}${sysconfdir}/nfc
	install -d ${D}${sysconfdir}/nfc/devices.d
	install -d ${D}/lib/udev/rules.d/

	cp -r ${S}/contrib/udev/93-pn53x.rules ${D}/lib/udev/rules.d/
	install -m 0644 ${WORKDIR}/pn532_uart_on_rpi.conf ${D}${sysconfdir}/nfc/devices.d
	

}
