# Base this image on rpi-hwup-image
include recipes-core/images/rpi-hwup-image.bb

IMAGE_FEATURES += "package-management "
IMAGE_INSTALL += "\
		plled \
		openssh \
		wpa-supplicant \
		usbutils \
		linux-firmware \
		wireless-tools \
		"
