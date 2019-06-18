SUMMARY = "Broadlink Python Common Libraries"
SECTION = "devel/python"
LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://LICENSE;md5=41e038530f86b1b37b29ec7c9c4a7218"

SRC_URI = "git://github.com/mjg59/python-broadlink.git"
SRCREV = "b6fd48db5d60d374eb4642b87757be56d42a83f7"

inherit setuptools3

S = "${WORKDIR}/git"

BBCLASSEXTEND = "native"
